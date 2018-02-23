#ifndef _CFG_H_
#define _CFG_H_


namespace CFG
{
	//read info
	const int LABEL_HEADER_LENGTH = sizeof(int) * 2;
	const int PIC_HEADER_LENGTH = sizeof(int) * 4;
	const int PIXEL_WIDTH = 28; //sqaure
	
	//labels
	const int N_LABELS = 60000;

	//draw info
	const int DISPLAY_SCALE = 10;

	//network
	const int N_INPUTS = PIXEL_WIDTH * PIXEL_WIDTH;
	const int N_OUTPUTS = 10;
	const int N_HIDDEN_LAYERS = 3;
	const int N_HIDDEN_NODES_P_LAYER = 3;
	const int N_TOTAL_WEIGHTS = N_INPUTS * N_HIDDEN_NODES_P_LAYER + (N_HIDDEN_LAYERS - 1) * N_HIDDEN_NODES_P_LAYER * N_HIDDEN_NODES_P_LAYER + N_OUTPUTS * N_HIDDEN_NODES_P_LAYER;
	const int N_TOTAL_BIASES = N_HIDDEN_NODES_P_LAYER * N_HIDDEN_LAYERS + N_OUTPUTS;

	//training
	const int BATCH_SIZE = 3;
	const float STEP_SIZE = .05;
	const int N_BATCHES = 3;
}

#endif // _CFG_H