#include <cmath>
#include <string>
#include <iostream>
#include <cmath>
#include <math.h>
#include <random>
#define PI 3.14159265


int main(){
  double lower_bound=0.;
  double upper_bound=1.;
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::random_device r;
  std::default_random_engine e1(r());
  double a_random_double = unif(e1);
  std::cout<<"a random double: "<<a_random_double<<std::endl;
  /*
  int rndnum = rand()%100;
  */
  /*
  double current_track_length = 90.;
  double efficiency = ((atan(0.05*(current_track_length - 30.)))/PI) + 0.5;
  std::cout<<"efficiency: "<<efficiency<<std::endl;
  */
  /*
  double m_p = 0.938;
  double m_n = 0.940;
  double E_B = 0.306;
  double m_mu = 0.1057;
  double this_lepton_energy = 0.7;
  double this_lepton_P = 0.6;
  double this_lepton_cos_theta = 0.5;
  double this_reco_energy_top = pow(m_p,2.) - pow((m_n - E_B),2.) - pow(m_mu,2.)+2*(m_n - E_B)*this_lepton_energy;
  double this_reco_energy_bottom = (m_n - E_B)-this_lepton_energy + this_lepton_P * this_lepton_cos_theta;
  double this_reco_energy = 0.5*(this_reco_energy_top/this_reco_energy_bottom);
  std::cout<<"top "<<this_reco_energy_top<<std::endl;
  std::cout<<"bottom "<<this_reco_energy_bottom<<std::endl;
  std::cout<<"reco energy "<<this_reco_energy<<std::endl;
  return 0;
  */

}
