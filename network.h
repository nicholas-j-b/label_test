/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * network.h
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

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <string.h>
#include "cfg.h"
#include <math.h>
#include <cmath>

class Network
{
public:
	Network();
	~Network();

	void read_batch(int batchID);

	void parse_input();

	void init_random_network();
	void do_batch();
	void clear_activations();
	void back_prop();
	 
protected:
	float** weights;
	float** biases;
	 
	char* inputHold;
	char* labelHold;

	float* inputs;
	float** activations;
	float* outputs;

	int* batchLabels;
	float* batchInputHold;
	float* batchOutputHold;

	int currentBatch;

	int* y;

	std::streampos label_pos;
	std::streampos pic_pos;

private:

};

#endif // _NETWORK_H_

