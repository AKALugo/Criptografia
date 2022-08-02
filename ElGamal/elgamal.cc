#include <math.h>
#include <iostream>
#include <vector>

int
ExponenciacionRapida(int base, int exponente, int modulo) {
  int x = 1;
  int y = base % modulo;

  while (exponente > 0 && y > 1) {
    if (exponente % 2 == 1) {
      x = (x * y) % modulo;
      exponente = exponente - 1;
    }
    y = (y * y) % modulo;
    exponente = exponente / 2;
  }
  return x;
}

int 
ModuloNegativo(int valor, int modulo) {
  
  while (valor < 0) {
    valor += modulo;
  }

  return valor;
}


int
EuclidesExtendido(int dividendo, int divisor) {

  std::vector<int> x, z;
  int iteracion = 2;
  x.push_back(-1); x.push_back(dividendo); x.push_back(divisor);
  z.push_back(0); z.push_back(1);

  while(x[iteracion - 1] % x[iteracion] != 0) {
    x.push_back(x[iteracion - 1] % x[iteracion]);
    z.push_back(ModuloNegativo(-1 * (x[iteracion - 1] / x[iteracion]) * z[iteracion - 1] + z[iteracion - 2], dividendo));
    iteracion++;
  }

  if (x[iteracion] == 1) return z[iteracion - 1];
  else return -1;
}



int main(void) {

  int p, a, k, x, m;
  std::cout << "p = ";
  std::cin >> p;

  std::cout << "a = ";
  std::cin >> a;

  std::cout << "k = ";
  std::cin >> k;

  std::cout << "x = ";
  std::cin >> x;

  std::cout << "m = ";
  std::cin >> m;

  std::cout << "\n\nBob:\n";
  int vpB = ExponenciacionRapida(a, x, p);
  std::cout << "Calculamos el valor público de Bob:" << vpB << std::endl;

  std::cout << "Alice:\n";
  int KA = ExponenciacionRapida(vpB, k, p);
  std::cout << "Calculamos la clave compartida con Bob (utilizando el valor público de Bob):" << KA << std::endl;
  int C = m * KA % p;
  std::cout << "Calculamos el mensaje cifrado:" << C << std::endl;
  int vpA = ExponenciacionRapida(a, k, p);
  std::cout << "Calculamos el valor público de Alice:" << vpA << std::endl;

  std::cout << "Bob:\n";
  int KB = ExponenciacionRapida(vpA, x, p);
  std::cout << "Calculamos la clave compartida con Alice (utilizando el valor público de Alice):" << KB << std::endl;

  int KAlaMenos1 = EuclidesExtendido(p, KB);
  if (KAlaMenos1 == -1) {
    std::cout << "No se puede descifrar el mensaje, " << p << " y " << KB << " no son coprimos" << std::endl;
  } else {
    std::cout << "Calculamos la inversa de la clave compartida con Alice:" << KAlaMenos1 << std::endl;

    int m2 = C * KAlaMenos1 % p;
    std::cout << "Calculamos el mensaje descifrado:" << m2 << std::endl << std::endl;

    std::cout << "En resumen:";
    std::cout << "yA = " << vpA << ", ";
    std::cout << "yB = " << vpB << ", ";
    std::cout << "K = " << KB << ", ";
    std::cout << "C = " << C << ", ";
    std::cout << "K-1 = " << KAlaMenos1 << ", ";
    std::cout << "M = " << m2 << "\n";
  }
}