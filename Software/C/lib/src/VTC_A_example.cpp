#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>

using namespace std;

#include  "VTC_A_lib.h"


const string PATH = "../../../python_plot";
const int32_t size_osc = 16384;
const int32_t nch = 32;


class DGTZ {
public:
	NI_HANDLE handle;
	string filename;
	string ip;
	bool connected;


	DGTZ(string _ip, string _filename) {
		ip = _ip;
		filename = _filename;
	}

	int Connect() {
		connected = false;
		if (R_ConnectDevice((char*)ip.c_str(), 8888, &handle) != 0) { return (-1); };
		connected = true;
		return 0;
	}
};

void DGTZ_Thread(void *context) {
	DGTZ *cntx;
	cntx = (DGTZ*)context;
	FILE *fp;
	int ret = fopen_s(&fp, cntx->filename.c_str(), "w");
	if (ret)return ;
	int32_t pre_trigger = 100;
	uint32_t status_osc = 0;
	uint32_t read_data_osc;
	uint32_t valid_data_osc;
	int32_t position = 0;

	int32_t timeout_osc = 1000;

	uint32_t *data_osc = (uint32_t*)malloc(2*size_osc*nch * sizeof(uint32_t));
	uint32_t *read_analog = (uint32_t*)malloc(2*size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital0 = (uint32_t*)malloc(2*size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital1 = (uint32_t*)malloc(2*size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital2 = (uint32_t*)malloc(2*size_osc*nch * sizeof(uint32_t));
	uint32_t *read_digital3 = (uint32_t*)malloc(2*size_osc*nch * sizeof(uint32_t));

	if ((data_osc == NULL) ||
		(read_analog == NULL) ||
		(read_digital0 == NULL) ||
		(read_digital1 == NULL) ||
		(read_digital2 == NULL) ||
		(read_digital3 == NULL))
		return;

	int Osc_Events = 10;
	int e = 0;

	char line_string[4096];




	while (status_osc != 1)
		if (OSCILLOSCOPE_Oscilloscope_0_STATUS(&status_osc, &cntx->handle) != 0) cout << "Status Error" << endl;
	if (OSCILLOSCOPE_Oscilloscope_0_POSITION(&position, &cntx->handle) != 0) cout << "Position Error" << endl;
	if (OSCILLOSCOPE_Oscilloscope_0_DOWNLOAD(data_osc, size_osc*nch, timeout_osc, &cntx->handle, &read_data_osc, &valid_data_osc) != 0) cout << "Get Data Error" << endl;
	if (OSCILLOSCOPE_Oscilloscope_0_RECONSTRUCT(data_osc, position, pre_trigger, read_analog, read_digital0, read_digital1, read_digital2, read_digital3) != 0) cout << "Reconstruction Error" << endl;

	for (int i = 0; i < size_osc-1; i++) {
		line_string[0] = '\0';
		for (int n = 0; n < nch; n++) {
			sprintf_s(line_string + strlen(line_string), 2048, "%8d", read_analog[(n*size_osc) + i]);
		}
		sprintf_s(line_string + strlen(line_string), 2048, "\n");
		fputs(line_string, fp);
	}
	e++;
	
	fclose(fp);
	
}

int main(int argc, char* argv[])
{
	vector<DGTZ> dgtz;

	int32_t pre_trigger = 100;
	int32_t decimator = 0;
	int32_t software_trigger = 0;
	int32_t analog_trigger = 0;
	int32_t digital0_trigger = 0;
	int32_t digital1_trigger = 0;
	int32_t digital2_trigger = 0;
	int32_t digital3_trigger = 0;
	int32_t trigger_channel = 0;
	int32_t trigger_edge = 0;
	int32_t trigger_level = 1000;
	vector<thread> threads;

	R_Init();


	dgtz.push_back(DGTZ("192.168.102.168", PATH + "/w1.txt"));
	dgtz.push_back(DGTZ("192.168.102.110", PATH + "/w2.txt"));
	dgtz.push_back(DGTZ("192.168.102.105", PATH + "/w3.txt"));
	dgtz.push_back(DGTZ("192.168.102.182", PATH + "/w4.txt"));


	for (int i = 0; i < dgtz.size(); i++) {
		if (dgtz[i].Connect() == 0) {
			cout << "Connected to " << dgtz[i].ip << endl;
		}else{
			cout << "Unable to connect to the board " << dgtz[i].ip << endl;
			exit(-1);
		}
	}


	for (int i = 0; i < dgtz.size(); i++) {
		if (OSCILLOSCOPE_Oscilloscope_0_SET_PARAMETERS(decimator, pre_trigger, software_trigger, analog_trigger, digital0_trigger, digital1_trigger,
			digital2_trigger, digital3_trigger, trigger_channel, trigger_edge, trigger_level, &dgtz[i].handle) != 0) {
			cout << "Unable to configure board " << dgtz[i].ip << endl;
		}
		else {
			cout << "Board " << dgtz[i].ip << " configured." << endl ;
		}
	}

	SetSyncRegister(0, 0, &dgtz[0].handle);
	for (int i = 0; i < dgtz.size(); i++) {
		if (OSCILLOSCOPE_Oscilloscope_0_START(&dgtz[i].handle) != 0) {
			cout << "Unable to start board " << dgtz[i].ip << endl;
		}
		//threads.emplace_back(DGTZ_Thread, ::std::ref(d));
		threads.push_back( thread(DGTZ_Thread,  (void*) (&dgtz[i])));
		
	}

	SetSyncRegister(0, 1, &dgtz[0].handle);

	
	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
	SetSyncRegister(0, 0, &dgtz[0].handle);
	return 0;
}

 