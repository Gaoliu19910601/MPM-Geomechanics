/*
 * SolverExplicitTwoPhaseUSL.cpp
 *
 *  Created on: 2 de ago de 2021
 *      Author: Fabricio Fernandez <fabricio.hmf@gmail.com>
 */

#include "Solver/SolverExplicitTwoPhaseUSL.h"
#include "Model.h"
#include "Integration.h"
#include "Interpolation.h"
#include "Update.h"
#include "Output.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;

SolverExplicitTwoPhaseUSL::SolverExplicitTwoPhaseUSL():Solver() { }

void SolverExplicitTwoPhaseUSL::Solve( ){

	// check if is two-phase calculations
	if(!ModelSetup::getTwoPhaseActive()) return;

	// simulation variables
	double time=ModelSetup::getTime();
	double dt=ModelSetup::getTimeStep();
	int resultSteps=ModelSetup::getResultSteps();
	double iTime=0.0;
	int loopCounter=0;

	// solve in time
	while(iTime<time) {

		// write results
		if (loopCounter++%resultSteps==0){

			Output::writeBodies(bodies,iTime);
			cout<<"Time = "<<iTime<<"\n";
		}

		// update contribution nodes
		Update::contributionNodes(mesh,bodies);
		
		#pragma omp parallel sections num_threads(4)
		{
			// nodal mass
			#pragma omp section
			Interpolation::nodalMass(mesh,bodies);

			// nodal mass of fluid
			#pragma omp section
			Interpolation::nodalMassFuid(mesh,bodies);

			// nodal momentum
			#pragma omp section
			Interpolation::nodalMomentum(mesh,bodies);

			// nodal momentum of fluid
			#pragma omp section
			Interpolation::nodalMomentumFluid(mesh,bodies);
		}
		
		// impose essential boundary condition on nodal momentum in mixture
		#pragma omp parallel sections num_threads(2)
		{
			// impose momentum boundary condition in solid phase
			#pragma omp section
			Update::boundaryConditionsMomentum(mesh);

			// impose momentum boundary condition in fluid phase
			#pragma omp section
			Update::boundaryConditionsMomentumFluid(mesh);
		}

		#pragma omp parallel sections num_threads(4)
		{
			// nodal internal force
			#pragma omp section
			Interpolation::nodalInternalForce(mesh,bodies);

			// nodal internal force of fluid phase
			#pragma omp section
			Interpolation::nodalInternalForceFluid(mesh,bodies);
		
			// nodal external force
			#pragma omp section
			Interpolation::nodalExternalForce(mesh,bodies);

			// nodal external force of fluid phase
			#pragma omp section
			Interpolation::nodalExternalForceFluid(mesh,bodies);
		}

		// nodal total force in mixture
		Update::nodalTotalForce(mesh);

		#pragma omp parallel sections num_threads(2)
		{
			// impose force boundary conditions
			#pragma omp section
			Update::boundaryConditionsForce(mesh);

			// impose force boundary conditions
			#pragma omp section
			Update::boundaryConditionsForceFluid(mesh);
		}

		// integrate the grid nodal momentum equation in mixture
		Integration::nodalMomentum(mesh,loopCounter==1?dt/2.0:dt);

		// update particle velocity of solid phase
		Update::particleVelocity(mesh,bodies,loopCounter==1?dt/2.0:dt);
		
		// update particle velocity of fluid phase
		Update::particleVelocityFluid(mesh,bodies,loopCounter==1?dt/2.0:dt);

		// update particle position of solid phase
		Update::particlePosition(mesh,bodies,dt);

		// nodal velocity
		Update::nodalVelocity(mesh);

		#pragma omp parallel sections num_threads(2)
		{
			// calculate particle strain increment
			#pragma omp section
			Interpolation::particleStrainIncrement(mesh,bodies,dt);
		
			// calculate particle vorticity increment
			#pragma omp section
			Interpolation::particleVorticityIncrement(mesh,bodies,dt);
		}

		// update particle density
		Update::particleDensity(bodies);
		
		// update particle stress
		Update::particleStress(bodies);
		
		// reset all nodal values
		Update::resetNodalValues(mesh);

		// advance in time
		iTime+=dt;
	}

	// write the Eulerian mesh
	Output::writeGrid(mesh,Output::CELLS);

	// write results series
	Output::writeResultsSeries();
}


