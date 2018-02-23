/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * network.cc
 * Copyright (C) 2017 Nicholas <>
 *
 * labels_test is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * labels_test is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "network.h"

Network::Network()
:currentBatch(0)
{
	srand(time(NULL));
	
	//allocate memory for weights and biases
	weights = new float*[CFG::N_HIDDEN_LAYERS + 1];
	biases = new float*[CFG::N_HIDDEN_LAYERS + 1];

	
	weights[0] = new float[CFG::N_INPUTS * CFG::N_HIDDEN_NODES_P_LAYER];
	for (int i = 1; i < CFG::N_HIDDEN_LAYERS; i++){
		weights[i] = new float[CFG::N_HIDDEN_NODES_P_LAYER * CFG::N_HIDDEN_NODES_P_LAYER];
	}
	weights[CFG::N_HIDDEN_LAYERS] = new float[CFG::N_OUTPUTS * CFG::N_HIDDEN_NODES_P_LAYER];

	
	for (int i = 0; i < CFG::N_HIDDEN_LAYERS; i++){
		biases[i] = new float[CFG::N_HIDDEN_NODES_P_LAYER];
	}
	biases[CFG::N_HIDDEN_LAYERS] = new float[CFG::N_OUTPUTS];


	//allocate for activations etc
	inputs = new float[CFG::N_INPUTS];
	outputs = new float[CFG::N_OUTPUTS];
	activations = new float*[CFG::N_HIDDEN_LAYERS];
	for (int i = 0; i < CFG::N_HIDDEN_LAYERS; i++){
		activations[i] = new float[CFG::N_HIDDEN_NODES_P_LAYER];
	}


	//init file reading, streampos skipping headers
	label_pos = 8;
	pic_pos = 16;
	inputHold = new char[CFG::BATCH_SIZE * CFG::N_INPUTS];
	labelHold = new char[CFG::BATCH_SIZE];

	batchLabels = new int[CFG::BATCH_SIZE];
	batchInputHold = new float[CFG::N_INPUTS * CFG::BATCH_SIZE];
	batchOutputHold = new float[CFG::N_OUTPUTS * CFG::BATCH_SIZE];

	y = new int[CFG::BATCH_SIZE * CFG::N_OUTPUTS];

}

Network::~Network()
{

	//delete weights and biases
	for (int i = 0; i < CFG::N_HIDDEN_LAYERS + 1; i++){
		delete[] weights[i];
	}

	for (int i = 0; i < CFG::N_HIDDEN_LAYERS + 1; i++){
		delete[] biases[i];
	}
	delete[] weights;
	delete[] biases;


	//activations etc
	delete[] inputs;
	delete[] outputs;
	for (int i  = 0; i < CFG::N_HIDDEN_LAYERS; i++){
		delete[] activations[i];
	}
	delete[] activations;

	//delete holds
	delete[] inputHold;
	delete[] labelHold;
	
	delete[] batchLabels;
	delete[] batchOutputHold;
	delete[] batchInputHold;

	delete[] y;

}


void Network::read_batch(int batchID)
{
	currentBatch = batchID; //not currently used???
	//read pics
	std::ifstream picData("/home/nicholas/data/train-images.idx3-ubyte", std::ios::in | std::ios::binary);
	if(picData.is_open()){
		picData.seekg(pic_pos);
		picData.read(inputHold, CFG::BATCH_SIZE * CFG::PIXEL_WIDTH * CFG::PIXEL_WIDTH);
		pic_pos = picData.tellg();
	} else {
		throw;
	}
	picData.close();

	//read labels
	std::ifstream labData("/home/nicholas/data/train-labels.idx1-ubyte", std::ios::in | std::ios::binary);
	if(labData.is_open()){
		labData.seekg(label_pos);
		labData.read(labelHold, CFG::BATCH_SIZE);
		label_pos = labData.tellg();
	} else {
		throw;
	}
	labData.close();
}


//sets up batch holds at beginning of each batch
void Network::parse_input()
{
	for (int i = 0; i < CFG::BATCH_SIZE * CFG::N_INPUTS; i++){
		batchInputHold[i] = ((float)static_cast<unsigned char>(inputHold[i])) / 256.0f;
	}
	for (int i = 0; i < CFG::BATCH_SIZE; i++){
		batchLabels[i] = (int)reinterpret_cast<unsigned char&>(labelHold[i]);
		std::cout<<"lab"<<std::endl;
		std::cout<<batchLabels[i]<<std::endl;
	}
}

//TODO:STILL NEED TO ABSTRACT OUT INITIATION//
void Network::init_random_network()
{
	//init random weights
	//first column
	for (int i = 0; i < CFG::N_HIDDEN_NODES_P_LAYER; i++){
		for (int j = 0; j < CFG::N_INPUTS; j++){
			weights[0][i * CFG::N_INPUTS + j] = ((float)rand() / ((float)RAND_MAX * 0.5)) - 1.0;
		}
	}
	
	//middle columns
	for (int i = 0; i < CFG::N_HIDDEN_LAYERS; i++){
		for (int j = 0; j < CFG::N_HIDDEN_NODES_P_LAYER; j++){
			for (int k = 0; k < CFG::N_HIDDEN_NODES_P_LAYER; k++){
				weights[i][j * CFG::N_HIDDEN_NODES_P_LAYER + k] = ((float)rand() / ((float)RAND_MAX * 0.5)) - 1.0;
			}
		}
	}

	//last column
	for (int i = 0; i < CFG::N_OUTPUTS; i++){
		for (int j = 0; j < CFG::N_HIDDEN_NODES_P_LAYER; j++){
			weights[CFG::N_HIDDEN_LAYERS][i * CFG::N_HIDDEN_NODES_P_LAYER + j] = ((float)rand() / ((float)RAND_MAX * 0.5)) - 1.0;
		}
	}

	//init random biases
	for (int i = 0; i < CFG::N_HIDDEN_LAYERS; i++){
		for (int j = 0; j < CFG::N_HIDDEN_NODES_P_LAYER; j++){
			biases[i][j] = ((float)rand() / ((float)RAND_MAX * 0.5)) - 1.0; //consider...
		}
	}

	for (int i = 0; i < CFG::N_OUTPUTS; i++){
		biases[CFG::N_HIDDEN_LAYERS][i] = (((float)rand() / ((float)RAND_MAX * 0.5)) - 1.0) * 1;//consider the weight of the bias may need new for loop
	}
}

//TODO: clean this up lol
void Network::do_batch()
{
	for (int i = 0; i < CFG::BATCH_SIZE; i++){
		clear_activations(); // perhaps unnecessary
		float test0 = 0;
		float test1 = 0;
		float test2 = 0;
		float test3 = 0;
		float testWeights = 0;

		//read inputs
		for (int j = 0; j < CFG::N_INPUTS; j++){
			inputs[j] = batchInputHold[i * CFG::N_INPUTS + j];
			test0 += inputs[j];
		}

		//do first layer
		for (int j = 0; j < CFG::N_HIDDEN_NODES_P_LAYER; j++){
			activations[0][j] = biases[0][j];
			for (int k = 0; k < CFG::N_INPUTS; k++){
				activations[0][j] += inputs[k] * weights[0][j * CFG::N_INPUTS + k];
			}
			activations[0][j] /= 1 + fabs(activations[0][j]);
			test1 += activations[0][j];
		}
		
		//do middle layers
		for (int j = 0; j < CFG::N_HIDDEN_LAYERS - 1; j++){
			for (int k = 0; k < CFG::N_HIDDEN_NODES_P_LAYER; k++){
				activations[j + 1][k] = biases[j + 1][k];
				for (int l = 0; l < CFG::N_HIDDEN_NODES_P_LAYER; l++){
					activations[j + 1][k] += activations[j][l] * weights[j + 1][k * CFG::N_HIDDEN_NODES_P_LAYER + l];
				}
				activations[j + 1][k] /= 1 + fabs(activations[j + 1][k]);
			}
		}
		
		//do last layer
		for (int j = 0; j < CFG::N_OUTPUTS; j++){
			//outputs[j] = 0;
			outputs[j] = biases[CFG::N_HIDDEN_LAYERS][j]; //could this be removed? seems detrimental
			for (int k = 0; k < CFG::N_HIDDEN_NODES_P_LAYER; k++){
				outputs[j] += activations[CFG::N_HIDDEN_LAYERS - 1][k] * weights[CFG::N_HIDDEN_LAYERS][j * CFG::N_HIDDEN_NODES_P_LAYER + k];
			}
			outputs[j] /= 1 + fabs(outputs[j]);
			//relu
			//outputs[j] = (outputs[j] + fabs(outputs[j])) / 2;
		}

		//put outputs in batch hold
		for (int j = 0; j < CFG::N_OUTPUTS; j++){
			batchOutputHold[i * CFG::N_OUTPUTS + j] = outputs[j];
		}

		std::cout<<"\noutputs: "<<std::endl;
		//print outputs
		for (int p = 0; p < CFG::N_OUTPUTS; p++){
			std::cout<<outputs[p]<<std::endl;
		}
		std::cout<<""<<std::endl;
	}
}


void Network::back_prop()
{
	//setup ys
	for (int i = 0; i < CFG::BATCH_SIZE; i++){
		for (int j = 0; j < CFG::N_OUTPUTS; j++){
			y[i * CFG::N_OUTPUTS + j] = 0;
		}
	}
	for (int i = 0; i < CFG::BATCH_SIZE; i++){
		y[i * CFG::N_OUTPUTS + batchLabels[i]] = 1;
	}

	//c_0
	float c_0 = 0;
	for (int i = 0; i < CFG::BATCH_SIZE; i++){
		for (int j = 0; j < CFG::N_OUTPUTS; j++){
			c_0 += pow(batchOutputHold[i * CFG::N_OUTPUTS + j] - y[i * CFG::N_OUTPUTS + j], 2);
		}
	}

	for (int i = 0; i < 
	
	
}



	




void Network::clear_activations()
{
	for (int i = 0; i < CFG::N_HIDDEN_LAYERS; i++){
		for (int j = 0; j < CFG::N_HIDDEN_NODES_P_LAYER; j++){
			activations[i][j] = 0;
		}
	}
}























	