/*
 * testInterpolation.cpp
 *
 *  Created on: 1 de mai de 2021
 *      Author: Fabricio Fernandez <fabricio.hmf@gmail.com>
 */


#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <fstream>
using std::ofstream;

#include <string>
using std::to_string;

#include "MPM.h"
#include "Particle.h"
#include "Interpolation.h"
#include "Output.h"
#include "ShapeGimp.h"

int main(int argc, char **argv)
{
	// create the mesh
	Mesh mesh;
	mesh.setNumCells(10,10,10);
	mesh.setCellDimension(1,1,1);
	mesh.createGrid();

	// create particles
	std::vector<Particle*> particles;
	particles.push_back(new Particle(Vector3d(0.0,0.0,0.0)));
	particles.push_back(new Particle(Vector3d(2.1,5.2,7.4)));
	particles.push_back(new Particle(Vector3d(1.4,3.2,8.4)));

	// configures particle
	for (size_t i=0; i<particles.size(); i++){

		// id
		particles.at(i)->setId(i);

		// size
		particles.at(i)->setSize(Vector3d(1.0,1.0,1.0));
		
		// mass
		particles.at(i)->setMass(1.0);

		// shape
		particles.at(i)->setShape(new ShapeGimp);
	}

	// update contributions nodes
	for (size_t i=0; i<particles.size(); i++){

		particles.at(i)->updateContributionNodes(mesh);
	}

	// interpolate particle mass to nodes
	Interpolation::nodalMass(mesh,particles);

	// open a file
	ofstream outputfile;
	outputfile.open ("interpolation.csv");
	outputfile<<"particleX,particleY,particleZ,nodeId,mass\n";

	// get grid nodes
	vector<Node> * gridNodes = mesh.getNodes();

	// write the nodal mass
	for (size_t i=0; i<particles.size(); i++){

		// contribution vector
		vector<Contribution> *contribution=particles.at(i)->getContributionNodes();
		
		// nodal mass
		for (size_t j = 0; j < contribution->size(); ++j)
		{
			outputfile
			   <<particles.at(i)->getPosition().x()<<
			","<<particles.at(i)->getPosition().y()<<
			","<<particles.at(i)->getPosition().z()<<
			","<<contribution->at(j).getNodeId()<<
			","<<gridNodes->at(contribution->at(j).getNodeId()).getMass()<<endl;
		}
	}
	outputfile.close();

	// write the mesh
	Output::writeGrid(mesh,Output::POINTS);

	// write the particles
	Output::writeParticles(particles);

	return 0;
}



