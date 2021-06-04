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

/// \namespace Output
/// \brief Operations to write the results and the mesh for its visualization.
namespace Output {

	/// \enum Fields
	/// \brief Defines all the keywords used in the out file.
	enum Fields
	{
		id, //!< write the particle id
		material, //!< write particle material id
		displacement //!< write particle displacement
	};

	/// \enum CellType
	/// Describes the way that the cells are written in the mesh file
	enum CellType{ POINTS, CELLS };

	/// \brief Initialize the keywords of the fields
	///
	void initFieldsKeyWords();

	/// \brief Write the grid mesh into a vtu file
	/// \param[in] mesh Mesh reference
	/// \param[in] cell_type_representation Cell type representation Output::CellType
	void writeGrid(Mesh& mesh, CellType cell_type_representation=CellType::CELLS);
	
	/// \brief Write the particles of the model into a vtu file
	/// \param[in] particles List containing pointers to particles
	/// \param[in] time Time
	void writeParticles(vector<Particle*>* particles, double time=0.0);
	
	/// \brief Write the particles in a body
	/// \param[in] body Body reference
	/// \param[in] time Time
	void writeBody(Body& body, double time=0.0);
	
	/// \brief Write the time series of the results
	///
	void writeResultsSeries();
	
	/// \brief Write the particles forming bodies
	/// \param[in] bodies Body list
	/// \param[in] time Time
	void writeBodies(vector<Body*>& bodies, double time=0.0);

	/// \brief Configures the fields to be written
	/// \param[in] fields List of fields
	void configureResultFiels(vector<string> fields);
};

#endif /* OUTPUT_H_ */
