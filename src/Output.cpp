/*
 * Output.cpp
 *
 *  Created on: 13 de abr de 2021
 *      Author: Fabricio Fernandez <fabricio.hmf@gmail.com>
 */

#if defined (linux) || defined(__linux__)
#include <sys/stat.h>
#endif

#if defined (_WIN64) || defined(_WIN32)
#include <direct.h>
#endif

#include "Output.h"

#include<string>
using std::string;
using std::to_string;

#include <fstream>
using std::ofstream;

#include <iomanip>
using std::fixed;
using std::scientific;

#include <vector>
using std::vector;

namespace OutputTolerance
{
    double deltaValue=1.0e-38;
}


namespace Folders
{
    string edian="";

    // grid
    bool gridFolderExist=false;
    string gridFolderName="grid";
    string gridFileName="eulerianGrid.vtu";
    
    // particles
    bool particleFolderExist=false;
    string particleFolderName="particles";
    string particleFileName="particles";
    vector<double> particleFilesTime;
}

Output::Output() {
    // TODO Auto-generated
}

Output::~Output() {
    // TODO Auto-generated destructor stub
}

void Output::defineEdian(){

    int16_t i = 1;
    int8_t *p = (int8_t*) &i;
    Folders::edian=(p[0]==1)?"LittleEndian":"BigEndian";
}

double Output::checkTolerance(double val){

    if (val==0.0) return val;
    if (val<0.0) return val>(-OutputTolerance::deltaValue)?(-OutputTolerance::deltaValue):val;
    return val<OutputTolerance::deltaValue?OutputTolerance::deltaValue:val;
}

void Output::createGridFolder(){

    if (Folders::gridFolderExist)
        return;
    
    int status=0;

    #if defined (linux) || defined(__linux__)
    status=mkdir(Folders::gridFolderName.c_str(),0777);
    #endif

    #if defined (_WIN64) || defined(_WIN32)
    status=_mkdir(Folders::gridFolderName.c_str());
    #endif

    if (status==-1)
        Folders::gridFolderExist=true;
}

void Output::createParticleFolder(){

    if (Folders::particleFolderExist)
        return;
    
    int status=0;

    #if defined (linux) || defined(__linux__)
    status=mkdir(Folders::particleFolderName.c_str(),0777);
    #endif

    #if defined (_WIN64) || defined(_WIN32)
    status=_mkdir(Folders::particleFolderName.c_str());
    #endif

    if (status==-1)
        Folders::particleFolderExist=true;
}

void Output::writeParticles(vector<Particle>& particles, double time){

    // define edian
    if(Folders::edian==""){
        defineEdian();
    }

    // create grid folder
    if(!Folders::particleFolderExist){
        createParticleFolder();
    }
    
    // add time in loop time vector
    Folders::particleFilesTime.push_back(time);

    // open particle file
    ofstream partFile;
    partFile.open((Folders::particleFolderName+"/"+Folders::particleFileName+"_"+to_string(Folders::particleFilesTime.size())+".vtu").c_str());
    partFile.precision(4);

    // particle data
    int nPoints=particles.size();

    // write results
    partFile<<"<?xml version=\"1.0\"?>\n";
    partFile<<"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\""<<Folders::edian.c_str()<<"\">\n";
    partFile<<"<UnstructuredGrid>\n";
    
    // piece
    partFile<<fixed<<"<Piece NumberOfPoints=\""<<nPoints<<"\" NumberOfCells=\""<<nPoints<<"\">\n";
    
    // points
    partFile<<"<Points>\n";
    
    // particle position
    partFile<<"<DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">\n";
    for (int i = 0; i < nPoints; ++i) {
        Vector3d pos=particles.at(i).getPosition();
        partFile<<scientific<<pos(0)<<" "<<pos(1)<<" "<<pos(2)<<"\n";
    }
    partFile<<"</DataArray>\n";

    // end points
    partFile<<"</Points>\n";
    
    // point data
    partFile<<"<PointData>\n";
    
    // particle Id
    partFile<<"<DataArray type=\"Float32\" Name=\"particleId\" Format=\"ascii\">\n";
    for (int i = 0; i < nPoints; ++i) {
        partFile<<scientific<<particles.at(i).getId()<<"\n";
    }
    partFile<<"</DataArray>\n";

    // end point data
    partFile<<"</PointData>\n";
    
    // cells
    partFile<<"<Cells>\n";

    // connectivity
    partFile<<"<DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">\n";
    for (int i = 0; i < int(particles.size()); ++i)
    {
        partFile<<i<<"\n";
    }
    partFile<<"</DataArray>\n";
    
    // offsets
    partFile<<"<DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">\n";
    for (int i = 0; i < int(particles.size()); ++i)
    {
        partFile<<i+1<<"\n";
    }
    partFile<<"</DataArray>\n";
    
    // types
    partFile<<"<DataArray type=\"UInt8\" Name=\"types\" Format=\"ascii\">\n";
    for (int i = 0; i < nPoints; ++i) {
        partFile<<1<<"\n";
    }
    partFile<<"</DataArray>\n";
    
    // end cells
    partFile<<"</Cells>\n";
    
    // end piece
    partFile<<"</Piece>\n";
    
    // end file
    partFile<<"</UnstructuredGrid>\n";
    partFile<<"</VTKFile>\n";

    // close file
    partFile.close();
}

void Output::writeGrid(Mesh& mesh, CellType gridType){

    // define edian
    if(Folders::edian==""){
        defineEdian();
    }

    // create grid folder
    if(!Folders::gridFolderExist){
        createGridFolder();
    }
    
    // open grid file
    ofstream gridFile;
    gridFile.open((Folders::gridFolderName+"/"+Folders::gridFileName).c_str());
    gridFile.precision(4);

    // mesh data
    int nPoints=mesh.getNumNodes();
    Vector3i nCellsVec = mesh.getTotalCells();
    int nCells=nCellsVec(0)*nCellsVec(1)*nCellsVec(2);

    // write results
    gridFile<<"<?xml version=\"1.0\"?>\n";
    gridFile<<"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\""<<Folders::edian.c_str()<<"\">\n";
    gridFile<<"<UnstructuredGrid>\n";
    
    // piece
    gridFile<<fixed<<"<Piece NumberOfPoints=\""<<nPoints<<"\" NumberOfCells=\""<<nCells<<"\">\n";
    
    // points
    gridFile<<"<Points>\n";
    
    // node position
    gridFile<<"<DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">\n";
    vector<Node>* inodes = mesh.getNodes();
    for (int i = 0; i < nPoints; ++i) {
        Vector3d pos=inodes->at(i).getCoordinates();
        gridFile<<scientific<<pos(0)<<" "<<pos(1)<<" "<<pos(2)<<"\n";
    }
    gridFile<<"</DataArray>\n";

    // end points
    gridFile<<"</Points>\n";
    
    // point data
    gridFile<<"<PointData>\n";
    
    // local ID of nodes
    gridFile<<"<DataArray type=\"Float32\" Name=\"Id\" Format=\"ascii\">\n";
    for (int i = 0; i < nPoints; ++i) {
        gridFile<<scientific<<inodes->at(i).getId()<<"\n";
    }
    gridFile<<"</DataArray>\n";

    // active nodes
    gridFile<<"<DataArray type=\"Float32\" Name=\"Active\" Format=\"ascii\">\n";
    for (int i = 0; i < nPoints; ++i) {
        gridFile<<scientific<<(inodes->at(i).getActive())<<"\n";
    }
    gridFile<<"</DataArray>\n";

    // nodal mass
    gridFile<<"<DataArray type=\"Float32\" Name=\"Mass\" Format=\"ascii\">\n";
    for (int i = 0; i < nPoints; ++i) {
        gridFile<<scientific<<(inodes->at(i).getMass())<<"\n";
    }
    gridFile<<"</DataArray>\n";

    // end point data
    gridFile<<"</PointData>\n";
    
    // cells
    gridFile<<"<Cells>\n";

    // connectivity
    gridFile<<"<DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">\n";
    
    int nCellsX = nCellsVec(0);
    int nCellsY = nCellsVec(1);
    int nCellsZ = nCellsVec(2);

    for (int k = 0; k < nCellsZ; ++k){
        for (int j = 0; j < nCellsY; ++j) {
            for (int i = 0; i < nCellsX; ++i){
                gridFile<<scientific
                <<k*(nCellsY+1)*(nCellsX+1)+j    *(nCellsX+1)+i                              <<" "
                <<k*(nCellsY+1)*(nCellsX+1)+j    *(nCellsX+1)+i+1                            <<" "
                <<k*(nCellsX+1)*(nCellsY+1)+(j+1)*(nCellsX)  +i+2+j                          <<" "
                <<k*(nCellsX+1)*(nCellsY+1)+(j+1)*(nCellsX)  +i+1+j                          <<" "
                <<k*(nCellsY+1)*(nCellsX+1)+j    *(nCellsX+1)+i    +(nCellsX+1)*(nCellsY+1)  <<" "
                <<k*(nCellsY+1)*(nCellsX+1)+j    *(nCellsX+1)+i    +(nCellsX+1)*(nCellsY+1)+1<<" "
                <<k*(nCellsX+1)*(nCellsY+1)+(j+1)*(nCellsX)  +i+2+j+(nCellsX+1)*(nCellsY+1)  <<" "
                <<k*(nCellsX+1)*(nCellsY+1)+(j+1)*(nCellsX)  +i+1+j+(nCellsX+1)*(nCellsY+1)  <<"\n";
            }
        }
    }
    gridFile<<"</DataArray>\n";
    
    // offsets
    gridFile<<"<DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">\n";
    for (int i = 0; i < nCells; ++i) {
        gridFile<<""<<8*(i+1)<< "\n";
    }
    gridFile<<"</DataArray>\n";
    
    // types
    int cellsType=gridType==CellType::POINTS?1:(gridType==CellType::CELLS?12:1);
    gridFile<<"<DataArray type=\"UInt8\" Name=\"types\" Format=\"ascii\">\n";
    for (int i = 0; i < nPoints; ++i) {
        gridFile<<cellsType<<"\n";
    }
    gridFile<<"</DataArray>\n";
    
    // end cells
    gridFile<<"</Cells>\n";
    
    // end piece
    gridFile<<"</Piece>\n";
    
    // end file
    gridFile<<"</UnstructuredGrid>\n";
    gridFile<<"</VTKFile>\n";

    // close file
    gridFile.close();
}

void Output::writeBody(Body& body, double time){

    writeParticles(body.getParticles(),time);
}
