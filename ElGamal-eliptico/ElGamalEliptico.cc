#include <math.h>
#include <iostream>
#include <vector>


int 
ModuloNegativo(int valor, int modulo) {
  
  while (valor < 0) {
    valor += modulo;
  }
  return valor % modulo;
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


std::pair<int,int>
sumadorDePuntos(std::pair<int, int> P, std::pair<int, int> Q, int a, int p) {

  int lamda, x3, y3;

  if (P == Q) {
    lamda = ((3 * P.first * P.first + a) * EuclidesExtendido(p, (2 * P.second))) % p;
  } else {
    lamda = (ModuloNegativo(Q.second - P.second, p) * EuclidesExtendido(p, ModuloNegativo(Q.first - P.first, p))) % p;
  }

  x3 = ModuloNegativo(lamda * lamda - P.first - Q.first, p);
  y3 = ModuloNegativo(lamda * (P.first - x3) - P.second, p);

  return std::make_pair(x3, y3);
}


std::pair<int, int>
sumadorDePuntosOptimo(std::pair<int, int> P, int iteraciones, int a, int p) {
  std::vector<std::pair<int, int>> puntos;
  puntos.push_back(P);

  while (pow(2, puntos.size()) <= iteraciones) {
    puntos.push_back(sumadorDePuntos(puntos[puntos.size() - 1], puntos[puntos.size() - 1], a, p));
  }

  std::pair<int, int> respuesta = puntos[puntos.size() - 1];
  int iteracionesRestantes = iteraciones - pow(2, puntos.size() - 1);

  if (iteracionesRestantes > 0) {
    for (int i = puntos.size() - 2; i >= 0; i--) {
      if (pow(2, i) <= iteracionesRestantes) {
        iteracionesRestantes -= pow(2, i);
        respuesta = sumadorDePuntos(respuesta, puntos[i], a, p);
      }
    }
  }

  return respuesta;
}


std::pair<int,int>
mensajeComoPunto(int m, int h, int p, std::vector<std::pair<int, int>> puntos) {
  
  int x, y = -1;

  int contador = 0;
  while (y == -1) {

    x = (m * h + contador) % p;
    for (unsigned i = 0; i < puntos.size(); i++) {
      if (puntos[i].first == x) {
        y = puntos[i].second;
        break;
      }
    }

    contador++;
  }

  return std::make_pair(x, y);
}

int
main(void) {

  int p, q, b, dB, dA, MensajeOriginal, M = 0, h;
  std::vector<int> x, y;
  std::vector<std::pair<int, int>> puntos;              
  std::pair<int, int> G, dBG, dAG, claveCompartidaA, claveCompartidaB, MensajePunto, MensajeCifrado;

  std::cout << "p = ";
  std::cin >> p;
  //p = 11;

  std::cout << "q = ";
  std::cin >> q;
  //a = 1;

  std::cout << "b = ";
  std::cin >> b;
  // b = 1;

  std::cout << "G1 = ";
  std::cin >> G.first;
  // G.first = 3;

  std::cout << "G2 = ";
  std::cin >> G.second;
  // G.second = 8;

  std::cout << "dB = ";
  std::cin >> dB;
  // dB = 2;

  std::cout << "dA = ";
  std::cin >> dA;
  // dA = 3;

  std::cout << "Mensaje original = ";
  std::cin >> MensajeOriginal;
  // MensajeOriginal = 3;

  if (p > 3) {
    long comprobacion = 4 * pow(q, 3) + 27 * b * b;
    if (comprobacion % p == 0) {
      std::cout << "Los valores de a y b no son adecuados\n";
      return 1;
    }
  }

  int auxX, auxY;
  std::cout << "Calculamos los valores de x e y\n";
  for (int i = 0; i < p; i++) {

    auxX = pow(i, 3) + q * i + b;
    auxY = pow(i, 2);
    x.push_back(auxX % p);
    y.push_back(auxY % p);
  }

  std::cout << "Calculamos los puntos de la curva: ";
  for (unsigned i = 0; i < x.size(); i++)
    for (unsigned j = 0; j < y.size(); j++)
      if (x[i] == y[j]) puntos.push_back(std::make_pair(i, j));
  
  for (unsigned i = 0; i < puntos.size(); i++)
    std::cout << "(" << puntos[i].first << "," << puntos[i].second << ") ";
  std::cout << "\n";



  std::cout << "Generamos la clave pública de B (dBG) = ";
  dBG = sumadorDePuntosOptimo(G, dB, q, p);

  std::cout << "(" << dBG.first << "," << dBG.second << ")\n";



  std::cout << "Generamos la clave pública de A (dAG) = ";
  dAG = sumadorDePuntosOptimo(G, dA, q, p);
  
  std::cout << "(" << dAG.first << "," << dAG.second << ")\n";



  std::cout << "Generamos la clave secreta compartida calculada por A = ";
  claveCompartidaA  = sumadorDePuntosOptimo(dBG, dA, q, p);

  std::cout << "(" << claveCompartidaA.first << "," << claveCompartidaA.second << ")\n";



  std::cout << "Generamos la clave secreta compartida calculada por B = ";
  claveCompartidaB = sumadorDePuntosOptimo(dAG, dB, q, p);

  std::cout << "(" << claveCompartidaA.first << "," << claveCompartidaA.second << ")\n";

  int contador = 0;
  while(M <= MensajeOriginal) {
    M = pow(2, contador);
    contador++;
  }
  std::cout << "Calculamos M = " << M << std::endl;



  h = p / M;
  std::cout << "Calculamos h = " << h << "<" << p << "/" << M << std::endl;



  MensajePunto = mensajeComoPunto(MensajeOriginal, h, p, puntos);
  std::cout << "Mensaje codificado como punto Qm = ";
    std::cout << "(" << MensajePunto.first << "," << MensajePunto.second << ")\n";



  std::cout << "Mensaje cifrado y clave publica enviados de A a B = ";
  MensajeCifrado = sumadorDePuntosOptimo(dBG, dA, q, p);

  MensajeCifrado = sumadorDePuntos(MensajeCifrado, MensajePunto, q, p);
  std::cout << "{(" << MensajeCifrado.first << "," << MensajeCifrado.second << ")" 
            <<",(" << dAG.first << "," << dAG.second << ")}\n";
}