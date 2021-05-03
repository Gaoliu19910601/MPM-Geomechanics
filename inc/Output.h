/*
 * Output.h
 *
 *  Created on: 13 de abr de 2021
 *      Author: Fabricio Fernandez <fabricio.hmf@gmail.com>
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "Mesh.h"
#include "Particle.h"
#include "Body.h"

/// \class Output
/// \brief Operations to write the results and the mesh for its visualization.
class Output {

public:
	
	/// \enum CellType
	/// Describes the way that the cells are written in the mesh file
	enum CellType{POINTS,CELLS};

	Output(); //!< default constructor
	virtual ~Output(); //!< default destructor
	void writeGrid(Mesh&,CellType=CellType::CELLS); //!< write the grid mesh into a vtu file.
	void writeParticles(vector<Particle>&, double=0.0); //!< write the particles of the model into a vtu file.
	void writeBody(Body&, double=0.0); //!< write the particles in a body.

private:

	void defineEdian(); //!< defines the type of data based of the system
	double checkTolerance(double); //!< defines a tolerance for write the results
	void createGridFolder(); //!< creates a folder containing the grid
	void createParticleFolder(); //!< create a folder containing the particles result files
};

#endif /* OUTPUT_H_ */
