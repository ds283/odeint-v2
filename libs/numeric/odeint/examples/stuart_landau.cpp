/*
 * stuart_landau.cpp
 *
 * This example demonstrates how one can use odeint can be used with state types consisting of complex variables.

 *  Created on: July 15, 2011
 *      Author: karsten
 *
 * Copyright 2009 Karsten Ahnert and Mario Mulansky.
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <complex>
#include <tr1/array>

#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;

//[ stuart_landau_system_function
typedef tr1::array< complex< double > , 1 > state_type;

struct stuart_landau
{
    double m_eta;
    double m_alpha;

    stuart_landau( double eta = 1.0 , double alpha = 1.0 )
    : m_eta( eta ) , m_alpha( alpha ) { }

    void operator()( const state_type &x , state_type &dxdt , double t ) const
    {
        const complex< double > I( 0.0 , 1.0 );
        dxdt[0] = ( 1.0 + m_eta * I ) * x[0] + ( 1.0 + m_alpha *I ) * norm( x[0] ) * x[0];
    }
};
//]


/*
//[ stuart_landau_system_function_alternative
double eta = 1.0;
double alpha = 1.0;

void stuart_landau( const state_type &x , state_type &dxdt , double t )
{
    const complex< double > I( 0.0 , 1.0 );
    dxdt[0] = ( 1.0 + m_eta * I ) * x[0] + ( 1.0 + m_alpha *I ) * norm( x[0] ) * x[0];
}
//]
*/


struct streaming_observer
{
    std::ostream& m_out;

    streaming_observer( std::ostream &out ) : m_out( out ) { }

    template< class State >
    void operator()( const State &x , double t ) const
    {
        m_out << t;
        for( size_t i=0 ; i<x.size() ; ++i ) m_out << "\t" << x[i];
        m_out << "\n";
    }
};




int main( int argc , char **argv )
{
    //[ stuart_landau_integration
    state_type x = {{ complex< double >( 1.0 , 0.0 ) }};

    const double dt = 0.1;

    typedef explicit_rk4< state_type > stepper_type;

    integrate_const( stepper_type() , stuart_landau() , x , 0.0 , 10.0 , dt , streaming_observer( cout ) );
    //]

    return 0;
}
