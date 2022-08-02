#include <math.h>
#include <iostream>
#include <vector>

int
ExponenciacionRapida(int base, int exponente, int modulo) {
  long x = 1;
  long y = base % modulo;

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


bool
testMillerRabin(int p, int k) {
  int s = -1, d;
  bool primo;
  do {
    s++;
    d = (p - 1) / pow(2, s);
  } while (d % 2 == 0);

  for (int i = 0; i < k; i++) {
    int a = 2 + rand()%(p - 2);
    
    primo = false;
    if (1 == ExponenciacionRapida(a, d, p)) {
      primo = true;
    } else {
      int r = 0;
      while(r <= s && !primo) {
        if (p - 1 == ExponenciacionRapida(a, d * pow(2, r), p)) {
          primo = true;
        }
        r++;
      }
    }
    if (!primo) return primo;
  }
  return primo;
}


bool
testLehmanPeralta(int p, int k) {
  int contador = 0;
  if (p == 2 || p == 3 || p == 5 || p == 7 || p == 11) return true;

  if (p % 2 == 0 || p % 3 == 0 || p % 5 == 0 || p % 7 == 0 ||
      p % 11 == 0) return false;
  
  for (int i = 0; i < k; i++) {
    int a = 2 + rand()%(p - 2);
    int r = ExponenciacionRapida(a, (p - 1) / 2, p);
    if (r == 1) contador++;
    else if (r != p - 1) return false;
  }

  if (contador == k) return false;
  return true;
}

int
main(void) {

  std::vector<int> bloques;
  int p, q, d, O, e, n, size;
  std::string textoOriginal, alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";;

  std::cout << "Texto original: ";
  std::getline(std::cin, textoOriginal);

  std::cout << "p = ";
  std::cin >> p;

  std::cout << "q = ";
  std::cin >> q;

  std::cout << "d = ";
  std::cin >> d;

  std::cout << "Se comprueba que p es primo\n";
  if (!testMillerRabin(p, 100)) {
    std::cout << "p no es primo\n";
    return 1;
  }

  std::cout << "Se comprueba que q es primo\n";
  if (!testLehmanPeralta(q, 100)) {
    std::cout << "q no es primo\n";
    return 1;
  }

  O = (p - 1) * (q - 1);
  std::cout << "Se calcula 0(n): " << O << "\n";

  std::cout << "Se comprueba que d es coprimo con O(n)\n";
  e = EuclidesExtendido(O, d);
  if (e == -1) {
    std::cout << "d no es coprimo con O(n)\n";
    return 1;
  }
  std::cout << "Calculamos e: " << e << "\n";

  n = p * q;
  size = log(n) / log(26);
  std::cout << "Calculamos n: " << n << ", se divide el texto en bloques de " << size << " caracteres\n";

  int contador = size - 1;
  int aux = 0;
  for (unsigned i = 0; i < textoOriginal.size(); i++) {
    int pos = alfabeto.find(textoOriginal[i]);
    if (contador >= 0 && pos != -1) {
      aux += pos * pow(26, contador);
      contador--;
    }
    if (contador < 0) {
      bloques.push_back(aux);
      aux = 0;
      contador = size - 1;
    }
  }
  
  if (contador != size - 1) {
    while (contador >= 0) {
      int pos = alfabeto.find('X');
      aux += pos * pow(26, contador);
      contador--;
    }
    bloques.push_back(aux);
  }

  std::cout << "Se pasa cada bloque a decimal para poder cifrar, obteniendo: ";
  for (unsigned i = 0; i < bloques.size(); i++) {
    std::cout << bloques[i] << " ";
  }

  std::cout << "\nSe calcula en decimal el texto cifrado: ";
  for (unsigned i = 0; i < bloques.size(); i++) {
    std::cout << ExponenciacionRapida(bloques[i], e, n) << " ";
  }
  std::cout << "\n";
}