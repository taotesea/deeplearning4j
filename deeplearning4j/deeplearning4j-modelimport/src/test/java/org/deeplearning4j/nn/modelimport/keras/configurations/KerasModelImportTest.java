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

package org.deeplearning4j.nn.modelimport.keras.configurations;

import lombok.extern.slf4j.Slf4j;
import org.deeplearning4j.BaseDL4JTest;
import org.deeplearning4j.nn.modelimport.keras.KerasModelImport;
import org.deeplearning4j.nn.modelimport.keras.exceptions.InvalidKerasConfigurationException;
import org.deeplearning4j.nn.modelimport.keras.exceptions.UnsupportedKerasConfigurationException;
import org.deeplearning4j.nn.multilayer.MultiLayerNetwork;
import org.junit.Test;
import org.nd4j.resources.Resources;

import java.io.IOException;

import static org.junit.Assert.assertNotNull;

/**
 * Test import of Keras models.
 */
@Slf4j
public class KerasModelImportTest extends BaseDL4JTest {

    @Test
    public void testH5WithoutTensorflowScope() throws Exception {
        MultiLayerNetwork model = loadModel("modelimport/keras/tfscope/model.h5");
        assertNotNull(model);
    }

    @Test
    public void testH5WithTensorflowScope() throws Exception {
        MultiLayerNetwork model = loadModel("modelimport/keras/tfscope/model.h5.with.tensorflow.scope");
        assertNotNull(model);
    }

    @Test
    public void testWeightAndJsonWithoutTensorflowScope() throws Exception {
        MultiLayerNetwork model = loadModel("modelimport/keras/tfscope/model.json",
                "modelimport/keras/tfscope/model.weight");
        assertNotNull(model);
    }

    @Test
    public void testWeightAndJsonWithTensorflowScope() throws Exception {
        MultiLayerNetwork model = loadModel(
                "modelimport/keras/tfscope/model.json.with.tensorflow.scope",
                "modelimport/keras/tfscope/model.weight.with.tensorflow.scope");
        assertNotNull(model);
    }

    private MultiLayerNetwork loadModel(String modelJsonFilename, String modelWeightFilename)
            throws NullPointerException {
        MultiLayerNetwork network = null;
        try {
            network = KerasModelImport.importKerasSequentialModelAndWeights(Resources.asFile(modelJsonFilename).getAbsolutePath(),
                    Resources.asFile(modelWeightFilename).getAbsolutePath(), false);
        } catch (IOException | InvalidKerasConfigurationException | UnsupportedKerasConfigurationException e) {
            log.error("",e);
        }

        return network;
    }

    private MultiLayerNetwork loadModel(String modelFilename) {
        MultiLayerNetwork model = null;
        try {
            model = KerasModelImport.importKerasSequentialModelAndWeights(Resources.asFile(modelFilename).getAbsolutePath());
        } catch (IOException | InvalidKerasConfigurationException | UnsupportedKerasConfigurationException e) {
            log.error("",e);
        }

        return model;
    }


}
