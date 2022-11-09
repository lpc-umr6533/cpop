/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef RANDOM_ENGINE_HH
#define RANDOM_ENGINE_HH

#include <CLHEP/Random/RandomEngine.h>

#include <assert.h>
#include <iostream>

#include <QString>

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The manager dealing with the random engine.
/// basically a simple call to the CLHEP engine with some add on functions
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////////////////////
class RandomEngineManager
{
public:
    /// \brief construcor
	RandomEngineManager();
    /// \brief destructor
	~RandomEngineManager();
	/// \brief return the singleton of the manager
	static RandomEngineManager* getInstance();

    /// \brief CLHEP engine setter
	void setEngine(CLHEP::HepRandomEngine*);
    /// \brief CLHEP engine getter
    CLHEP::HepRandomEngine* getEngine() const { return rndEngine;}
    
    /// \brief return a random double value between 0 and 1
    double randd();
    /// \brief return a random double value between min and max
    double randd(double min, double max);
    /// \brief return a value between 0 and RAND_MAX
    int randi();
    /// \brief return a random value between min and max
    int randi(int min, int max);
    /// \brief return the normal deviation
    double normaled(double min, double max, double medium, double standardDeviation);

    /// \brief pick a random element on the givent iterators
    /// \warning this function can be slow if your Elmt are separated from a large distance.
    /// the function pick random from a vector should be prefered if you need to pick randomly elemt frequentky   
    template<typename Elmt>
    Elmt pickRandom(Elmt begin, Elmt end )
    {
        assert(begin != end);
        const unsigned long n = std::distance(begin, end);
        int index = randi(0, n-1);  //< -1 : to avoid the "end"

        Elmt toAdvance = begin;
        std::advance(toAdvance, index);
        return toAdvance;
    }

    template<typename Elmt>
    Elmt pickRandom(const std::vector<Elmt>* pVector)
    {
        return pVector->at(randi(0, pVector->size()-1));
    }

    /// \brief return the current shoot made
    // int getIShoot() const { return iShoot;}

private:
	CLHEP::HepRandomEngine* rndEngine;     ///< \brief the CLHEP engine used
    std::ofstream* outputRandom;           ///< \brief used for debug
    // int iShoot;                            ///< \brief number of number shooted, used for random

};

#endif // RANDOM_ENGINE_HH