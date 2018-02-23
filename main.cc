/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
 * Copyright (C) 2017 Nicholas
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

#include <iostream>
#include <new>
#include "cfg.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "network.h"





int main()
{
	Network net;
	net.init_random_network();
	for (int i = 0; i < CFG::N_BATCHES; i++){
		net.read_batch(i);
		net.parse_input();
		net.do_batch();
		net.back_prop();
	}

	
	return 0;
}







int read_label()
{
	int nReadLabels = 10;
	int* header = new int[2];
	char* place = reinterpret_cast<char*>(header);
	char* labels = new char[nReadLabels];

	std::ifstream data("/home/nicholas/data/train-labels.idx1-ubyte", std::ios::in | std::ios::binary);
	if(data.is_open()){
		data.read(place, CFG::LABEL_HEADER_LENGTH);
		data.read(labels, nReadLabels);
	}
	data.close();

	for (int i = 0; i < nReadLabels; i++) {
		std::cout<<+labels[i]<<std::endl;
	}


	return 0;
}

unsigned char* read_pic()
{
	int* header = new int[4];
	char* headerBin = reinterpret_cast<char*>(header);
	char* pic = new char[CFG::PIXEL_WIDTH * CFG::PIXEL_WIDTH];

	std::ifstream data("/home/nicholas/data/train-images.idx3-ubyte", std::ios::in | std::ios::binary);
	if(data.is_open()){
		data.read(headerBin, CFG::PIC_HEADER_LENGTH);
		data.read(pic, CFG::PIXEL_WIDTH * CFG::PIXEL_WIDTH);
		data.read(pic, CFG::PIXEL_WIDTH * CFG::PIXEL_WIDTH);
		data.read(pic, CFG::PIXEL_WIDTH * CFG::PIXEL_WIDTH);
	}
	data.close();

	unsigned char* poc = reinterpret_cast<unsigned char*>(pic);

	return poc;
}

/*int main()
{
	unsigned char* pic = read_pic();
	read_label();
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP  *image   = NULL;

	if(!al_init()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	display = al_create_display(280, 280);

	if(!display) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
	}
	for (int i = 0; i < 28 * CFG::DISPLAY_SCALE; i++){
		for (int j = 0; j < 28 * CFG::DISPLAY_SCALE; j++){
			al_put_pixel(j, i, al_map_rgb(pic[(i / 10) * 28 + j / 10], pic[(i / 10) * 28 + j / 10], pic[(i / 10) * 28 + j / 10]));
		}
	}
	al_flip_display();
	al_rest(2);

	al_destroy_display(display);


	return 0;
}*/
