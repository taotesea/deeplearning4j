/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// Created by raver on 6/6/2018.
//

#include <system/op_boilerplate.h>
#include <system/pointercast.h>
#include <ops/declarable/BroadcastableBoolOp.h>
#include <helpers/ShapeUtils.h>

namespace sd {
    namespace ops {
        BroadcastableBoolOp::BroadcastableBoolOp(const char *name, int numTArgs, int numIArgs) : DeclarableCustomOp::DeclarableCustomOp(2, 1, name, false, numTArgs, numIArgs) {
            //
        }

        ShapeList *BroadcastableBoolOp::calculateOutputShape(ShapeList *inputShape, sd::graph::Context &block) {
            auto shapeList = SHAPELIST();
            auto x = inputShape->at(0);
            auto y = inputShape->at(1);
            sd::DataType dtype = sd::DataType::BOOL;

            if(shape::isEmpty(x) || shape::isEmpty(y)) {
                // this is edge case, [3, 4] + [] = []
                if ((shape::isEmpty(x) && shape::rank(x) == 0) || (shape::isEmpty(y) && shape::rank(y) == 0)) {
                    shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor::emptyDescriptor(dtype)));
                    return shapeList;
                }

                Nd4jLong *newshape = nullptr;
                ShapeUtils::evalBroadcastShapeInfo(x, y, true, newshape, block.workspace());
                shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(newshape, dtype)));
			} else if (shape::isScalar(x) && shape::isScalar(y)) {
                if (shape::rank(x) >= shape::rank(y)) {
                    shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(x, dtype)));
                } else {
                    shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(y, dtype)));
                }
            } else if (shape::equalsSoft(x, y)) {
                shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(x, dtype)));
            } else if (shape::isScalar(x) && !shape::isScalar(y)) {
                shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(y, dtype)));
            } else if (!shape::isScalar(x) && shape::isScalar(y)) {
                shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(x, dtype)));
            } else if (ShapeUtils::areShapesBroadcastable(x, y)) {
                Nd4jLong *newshape = nullptr;
                ShapeUtils::evalBroadcastShapeInfo(x, y, true, newshape, block.workspace());
                shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(newshape, dtype)));
            } else {
                // in this case we'll throw exception later
                shapeList->push_back(ConstantShapeHelper::getInstance()->createShapeInfo(ShapeDescriptor(x, dtype)));
            }

            return shapeList;
        }
    }
}