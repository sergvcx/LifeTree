#include "mainwindow.h"
#include <QApplication>
#include <QTreeView>
#include "taskmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;


    Task root("root");
    Task life("life",root);
        Task sport("sport",life,10,0);
            Task swim("swim",sport,10,0);
            Task pres("pres",sport,10,0);
                Task GoldsGym("GoldsGym",swim,0,10);
                swim.appendChild(GoldsGym);
                pres.appendChild(GoldsGym);
        Task dancing("dancing",life);
            Task hustle("хастл",dancing,1,0);
            Task zouk("зук",dancing,2,0);
                Task shoes("shoes",0,100);
                pres.appendChild(shoes);
                zouk.appendChild(shoes);
                hustle.appendChild(shoes);
        pres.appendChild(dancing);
        Task module("модуль",life,0,0);
            Task nmpp("nmpp",module,0,0);
                Task fft("fft",nmpp);
                    Task fft64("fft-64",fft,8);
                    Task ifft64("ifft-64",fft,8);
                    Task fft128("fft-128",fft,16);
                    Task ifft128("ifft-128",fft,8);
                    Task fft256("fft-256",fft,8);
                    Task ifft256("ifft-256",fft,8);
                    Task fft512("fft-512",fft,8);
                    Task ifft512("ifft-512",fft,8);
                    Task fft1024("fft-1024",fft,8);
                    Task ifft1024("ifft-1024",fft,8);
                    Task fft2048("fft-1024",fft,8);
                    Task ifft2048("ifft-1024",fft,8);

            Task jpeg("jpeg",module,0,0);
                Task jpegCom("jpeg-connect",jpeg,8);
                Task jpegOpt("jpeg-optimization",jpeg,16);
                Task jpegNMPP("jpeg-nmpp++",jpeg,8)


            Task matlab("matlab",module,100,0);
                Task fftRPC("fft-rpc",matlab);
                    Task fftRPC256("fft-rpc-256",fftRPC,1);
                        fftRPC256.appendChild(fft256);

                matlab.appendChild(fft256);


            dancing.appendChild(sport);
            module.appendChild(sport);
            sport.appendChild(dancing);

    TaskModel model(&root);
    QTreeView treeView;
    treeView.setModel(&model);

    treeView.show();
    return a.exec();
}

