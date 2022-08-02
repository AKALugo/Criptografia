#include <bitset>
#include <iostream>
#include <vector>

std::pair<int, int>
Satelite(int satelite) {
  std::vector<std::pair<int, int>> sateliteVector {
    std::make_pair(2, 6), std::make_pair(3, 7), std::make_pair(4, 8), std::make_pair(5, 9), 
    std::make_pair(1, 9), std::make_pair(2, 10), std::make_pair(1, 8), std::make_pair(2, 9), 
    std::make_pair(3, 10), std::make_pair(2, 3), std::make_pair(3, 4), std::make_pair(5, 6), 
    std::make_pair(6, 7), std::make_pair(7, 8), std::make_pair(8, 9), std::make_pair(9, 10),
    std::make_pair(1, 4), std::make_pair(2, 5), std::make_pair(3, 6), std::make_pair(4, 7), 
    std::make_pair(5, 8), std::make_pair(6, 9), std::make_pair(1, 3), std::make_pair(4, 6), 
    std::make_pair(5, 7), std::make_pair(6, 8), std::make_pair(7, 9), std::make_pair(8, 10), 
    std::make_pair(1, 6), std::make_pair(2, 7), std::make_pair(3, 8), std::make_pair(4, 9)};

  return sateliteVector[satelite];
}


// Desplazamos la secuencia.
std::bitset<10>
Desplazar(std::bitset<10>& LFSR, std::bitset<1>& aux) {


  std::bitset<10> swap;
  for (int i = LFSR.size() - 1; i >= 0; i--) {
    swap[i - 1] = LFSR[i];
  }
  
  swap[swap.size() - 1] = aux[0];
  return swap;
}


int main(void) {

  int satelite, tap1, tap2;
  unsigned secuencia;
  std::cout << "ID del satelite: ";
  std::cin >> satelite;
  // Buscamos los datos del satelite.
  tap1 = Satelite(satelite - 1).first;
  tap2 = Satelite(satelite - 1).second;

  std::cout << "longuitud de la secuencia de salida: ";
  std::cin >> secuencia;

  // Polinomios
  std::vector<int> pol1 {3}, pol2 {2, 3, 6, 8, 9};
  // Secuencia en binario
  std::bitset<10> LFSR1, LFSR2;
  // Bit que se desplaza.
  std::bitset<1> realimentacion1, realimentacion2, PRN1, CA_CODE;

  // Lo ponemos todo a 1.
  LFSR1.set();
  LFSR2.set();

  std::string pantalla = "";
  std::cout << "LFSR1      Realimentacion | LFSR2      Realimentacion | Secuencia C/A PRN1\n";

  // Lo hacemos tantas veces como se nos haya indicado por pantalla.
  for (unsigned iteraciones = 1; iteraciones <= secuencia; iteraciones++) {

    // Hacemos XOR segun haya escogido el satelite.
    PRN1[0] = LFSR2[LFSR2.size() - tap1] ^ LFSR2[LFSR2.size() - tap2];
    // Hacemos XOR con las posiciones del satelite y la última de la otra secuencia de bits.
    CA_CODE[0] = LFSR1[0] ^ PRN1[0];

    pantalla += LFSR1.to_string() + " ";
    // Guardamos el bit que se va a desplazar
    realimentacion1[0] = LFSR1[0];

    // Le hacemos XOR con todas las posiciones que se indique en el polinomio.
    for(unsigned i = 0; i < pol1.size(); i++) {
      realimentacion1[0] = realimentacion1[0] ^ LFSR1[LFSR1.size() - pol1[i]];
    }
    // Desplazamos la secuencia de bits.
    LFSR1 = Desplazar(LFSR1, realimentacion1);

    pantalla += realimentacion1.to_string() + "                ";



    pantalla += LFSR2.to_string() + " ";
    // Guardamos el bit que se va a desplazar
    realimentacion2[0] = LFSR2[0];
    
    // Le hacemos XOR con todas las posiciones que se indique en el polinomio.
    for(unsigned i = 0; i < pol2.size(); i++) {
        realimentacion2[0] = realimentacion2[0] ^ LFSR2[LFSR2.size() - pol2[i]];
    }
    // Desplazamos la secuencia de bits.
    LFSR2 = Desplazar(LFSR2, realimentacion2);

    pantalla += realimentacion2.to_string() + "                ";


    pantalla += CA_CODE.to_string();
    std::cout << pantalla << std::endl;

    pantalla = "";
  }
}
