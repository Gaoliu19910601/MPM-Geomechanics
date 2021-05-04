/*
 * Body.h
 *
 *  Created on: 13 de abr de 2021
 *      Author: Fabricio Fernandez <fabricio.hmf@gmail.com>
 */

#ifndef BODY_H_
#define BODY_H_

#include<vector>
using std::vector;

#include "Particle.h"

/// \class Body
/// \brief Represents a body
class Body {

public:
	
	Body();
	virtual ~Body();
	
	bool createCuboid(Mesh& mesh, Vector3d, Vector3d); //<! creates a rectangle 
	int getId(); //<! returns the body identification
	vector<Particle>& getParticles(); //<! returns particle in the body
	static int getTotalBodies(); //<! returns o number of bodies
	

private:
	
	int id; //!< body identification
	vector<Particle> particles; //!< material points forming the body  
	
	static int totalBodies; //!< number of bodies
};

#endif /* BODY_H_ */
