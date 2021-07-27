/*
 * Material.h
 *
 *  Created on: 13 de abr de 2021
 *      Author: Fabricio Fernandez <fabricio.hmf@gmail.com>
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

class Particle;

/// \class Material
/// \brief Represents a material.
class Material {

public:

	/// \enum MaterialType
	/// Describes the material's constitutive model type
	enum MaterialType{ NONE, ELASTIC, ELASTOPLASTIC };

	/// \brief Default constructor
	///
	Material(int id=-1,double density=0, MaterialType=MaterialType::NONE);
	
	/// \brief Default destructor
	///
	virtual ~Material();
	
	/// \brief Return the identification
	/// \return Material identification
	inline int getId() const { return this->id; }
	
	/// \brief Returns material density
	/// \return Material density
	inline double getDensity() const { return this->density; }

	/// \brief Configure the material identification
	/// \param[in] material_id Material identification
	inline void setId(int material_id) { this->id=material_id; }
	
	/// \brief Configures the material density
	/// \param[in] material_density Material density 
	inline void setDensity(double material_density) { this->density=material_density; }
	
	/// \brief Configures the material type
	/// \param[in] material_type Material::MaterialType
	inline void setType(MaterialType material_type) { this->type=material_type; }
	
	/// \brief Returns the material type
	/// \return Material::MaterialType
	inline virtual MaterialType getType() const { return this->type; }

	/// \brief Update the stress tensor
	/// \param[in] particle Particle
	virtual void updateStress(Particle* particle) const = 0;

	/// \brief Get sound speed
	/// \param[out] sound_speed Sound speed
	virtual double getSoundSpeed( ) const = 0;

protected:
	
	int id; //!< material identification

	double density; //!< density \f$\rho\f$
	
	MaterialType type; //!< material type
};

inline Material::Material(int id, double density, MaterialType type) {
	
	this->density=density;
	this->id=id;
	this->type=type;
}

inline Material::~Material() {

}

#endif /* MATERIAL_H_ */