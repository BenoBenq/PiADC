#include "TCanvas.h"
#include "TGraph.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TApplication.h"
#include <signal.h>
#include "./header/helper.h"

TCanvas *c1 = new TCanvas();

void signalEnd(int signo) {
    c1->Close();
    gSystem->ProcessEvents();
}

void plotRoot() {
    //TApplication rootapp("spectrum", &argc, argv);
    signal(SIGINT, signalEnd);
    TCanvas* c1 = new TCanvas();
    TGraph* gr1 = new TGraph();
    gr1->SetTitle("Real-time data");
    gr1->SetMinimum(0);
    gr1->SetMaximum(1024);
    gr1->Draw();

    int number_of_lines = 8000;
    char data[number_of_lines][1000];
    int values[number_of_lines];
    int time_value, value;
    while(true) {
        FILE *fp = fopen("recieve.d", "r");
        tail(fp, number_of_lines, data);
        fclose(fp);
        for(int i = 0; i < number_of_lines; i++) {
            sscanf(*(data+i), "%d %d", &time_value, &value);
            *(values+i) = value;
        }


        if(gROOT->GetListOfCanvases()->FindObject("c1") == nullptr) break;
        for(int i = 0; i < number_of_lines; i++) {
            gr1->SetPoint(i, i , *(values + i));
            //printf("Hello");
        }
        c1->Update();
        c1->Draw();
        usleep(100);
        gSystem->ProcessEvents();
    }

    c1->Close();
    gSystem->ProcessEvents();
    exit(0);
}
