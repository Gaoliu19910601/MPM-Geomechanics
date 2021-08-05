/*
 * MPM.h
 *
 *  Created on: 13 de abr de 2021
 *      Author: Fabricio Fernandez <fabricio.hmf@gmail.com>
 */

/// \mainpage MPM-Geomechanics Documentation
///
/// \section intro_sec Introduction
///
/// The MPM-Geomechanics is a Material Point Method implementation
/// using C++ and Object Oriented Programming to simulating 
/// geomechanical problems in 3D.
///
/// The main objectives of this program are:
///
/// - to provide a computational tool suitable for solve 
/// geomechanical problems involving large deformations.
///
/// - to provide a open-source framework for MPM simulations
/// in geomechanics.
///
/// \section install_sec Installation
///
/// \subsection step1 Step 1: ...
/// ...
///

#ifndef MPM_H_
#define MPM_H_

#include "Mesh/Mesh.h"
#include "Body/Body.h"
#include "Model.h"
#include "Output.h"
#include "Input.h"
#include "States.h"
#include "Solver/Solver.h"

/// \class MPM
/// \brief Components and algorithms of the MPM
/// This class contain the basic objects of a MPM model,
/// and the operations between them.
class MPM {

public:

	/// \brief Default constructor
	///
	MPM(); 
	
	/// \brief Default destructor
	///
	virtual ~MPM();

	/// \brief Read the input file of the model
	/// \param[in] argument_counter Argument counter
	/// \param[in] argument_vector Argument vector
	void readInputFile(int argument_counter, char ** argument_vector);
	
	/// \brief Create the numerical MPM model
	///
	void createModel();
	
	/// \brief Solve the problem in time
	///
	void solve();
	
	/// \brief Finish all program structure
	///
	void end();

private:
	
	Mesh mesh; //!< grid mesh

	vector<Body*> bodies; //!< bodies discretized by material points

	vector<Material*> materials; //!< bodies discretized by material points
	
	Solver* solver; //!< operation for solve the equations in time

	/// \brief Configure the simulation time
	///
	void setSimulationTime();
	
	/// \brief Configure the solver of the model
	///
	void setSolver();
	
	/// \brief Configure the mesh
	///
	void setupMesh();
	
	/// \brief Configures the material list
	///
	void setupMaterialList();
	
	/// \brief Configure the body list
	///
	void setupBodyList();
	
	/// \brief Create bodies
	///
	void createBodies();
	
	/// \brief Configure the time step 
	///
	void setTimeStep();

	/// \brief Configure the interpolation functions
	///
	void setInterpolationFunctions();
	
	/// \brief Configure particles
	///
	void setupParticles();
	
	/// \brief Configure loads in the model
	///
	void setupLoads();
	
	/// \brief Configure results to be written
	///
	void setupResults();
	
	/// \brief Configure dampig forces
	/// 
	void setupDamping();

	/// \bried Configure number of phases in the simulation
	///
	void setNumberPhasesInSimulation();

	/// \bried Configure number of threads
	///
	void setThreads();
};

#endif /* MPM_H_ */