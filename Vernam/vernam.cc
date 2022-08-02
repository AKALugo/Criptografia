
#include <iostream>
#include <string>
#include <vector>

#include <math.h>


// Método que calcula una clave de forma aleatoria
std::vector<u_int8_t>
Aleatorio (unsigned cantidad){

    srand(time(NULL));
    std::vector<u_int8_t> vector;
    vector.resize(cantidad);
    for (unsigned recorrido = 0; recorrido < vector.size(); recorrido++)
        vector[recorrido] = rand()%256; // Valores entre 0 y 255

    return vector;
} 

// Método que pasa a binario nuestros valores ASCII.
void
PasarBinario(u_int8_t numero) {

    std::string binario;

    // Dividimos el numero para pasarlo a binario
    while (numero != 0) {
       binario += (numero % 2 == 0 ? "0" : "1");
       numero /= 2;
    }
    // Rellenamos hasta que sean 8 valores.
    while (binario.size() != 8)
        binario += "0";

    // Mostramos por pantalla.
    for (int recorrido = binario.size() - 1; recorrido >= 0; recorrido--)
        std::cout << binario[recorrido];
}

// Método que cifra y descifra una cadena de texto según una clave.
void Cifrar_Descifrar (bool cifrar) {
    
    std::string mensaje, clave;
    std::vector<u_int8_t> cadena_bits_original, cadena_bits_clave, cadena_bits_cifrado;

    // Pedimos la cadena.
    if (cifrar) std::cout << "Introduzca el mensaje a cifrar: ";
    else        std::cout << "Introduzca el mensaje cifrado: ";
    std::cin.ignore();
    getline(std::cin, mensaje);
    std::cout << "\n";

    cadena_bits_original.resize(mensaje.size());
    cadena_bits_clave.resize(mensaje.size());
    cadena_bits_cifrado.resize(mensaje.size());

    // Pasamos cada letra a ASCII y guardamos en un unsigned de 8 bits.
    for (unsigned recorrer = 0; recorrer < mensaje.size(); recorrer++) {
        cadena_bits_original[recorrer] = u_int8_t(mensaje[recorrer]);
    }

    // Mostramos la cadena en binario gracias a su valor ASCII.
    if (cifrar) std::cout << "Mensaje original en binario: ";
    else        std::cout << "Mensaje cifrado en binario: ";
    for (unsigned recorrer = 0; recorrer < cadena_bits_original.size(); recorrer++)
        PasarBinario(cadena_bits_original[recorrer]);

    std::cout << std::endl << "Longuitud del mensaje en binario: " << 8 * mensaje.size() << "\n\n";

    bool clave_en_binario;

    // Pedimos una clave en binario y comprobamos que cumpla los requisitos.
    do {
        clave_en_binario = true;
        std::cout << "Introduzca la clave aleatoria: ";
        std::cin >> clave;

        // La clave aleatoria tiene que tener la misma longitud en binario que el mensaje original
        if (clave.size() != 8 * cadena_bits_original.size()) clave_en_binario = false;
        // Comprobamos que la clave solo sean 0 y 1
        for (unsigned recorrido = 0; recorrido < clave.size(); recorrido++)
            if (clave[recorrido] != '1' && clave[recorrido] != '0') clave_en_binario = false;

    } while (!clave_en_binario);

    // Pasamos la clave en binario a ASCII.
    int recorrido_s = 0;
    int contador;
    // Cogemos la clave en binario y separando de 8 en 8 con potencias de 2 sacamos el valor en decimal
    // que correspondería con el valor ASCII.
    for (unsigned recorrido = 0; recorrido < cadena_bits_clave.size(); recorrido++) {
        contador = 7;
        cadena_bits_clave[recorrido] = 0;
        while (contador >= 0) {
            if (clave[recorrido_s] == '1') cadena_bits_clave[recorrido] += pow(2, contador);
            recorrido_s ++;
            contador --;
        }
    }

    // Aplicamos el operador XOR entre los valores ASCII.
    for (unsigned recorrido = 0; recorrido < cadena_bits_cifrado.size(); recorrido++)
        cadena_bits_cifrado[recorrido] = cadena_bits_original[recorrido] ^ cadena_bits_clave[recorrido];

    // Mostramos el mensaje cifrado en binario.
    if (cifrar) std::cout << "\nEl mensaje cifrado en binario es: ";
    else        std::cout << "\nEl mensaje original en binario es: ";
    for (unsigned recorrer = 0; recorrer < cadena_bits_cifrado.size(); recorrer++)
        PasarBinario(cadena_bits_cifrado[recorrer]);

    // Pasamos el mesaje cifrado en ASCII a char.
    if (cifrar) std::cout << "\nEl mensaje cifrado es: ";
    else        std::cout << "\nEl mensaje original es: ";
    for (unsigned recorrer = 0; recorrer < cadena_bits_cifrado.size(); recorrer++)
        std::cout << cadena_bits_cifrado[recorrer];
    std::cout << "\n\n\n";
}



// Menú de nuestro programa.
int main (void) {

    std::string menu;
    while (true) {

        std::cout << "Introduzca:\n1) para cifrar un mensaje\n2) Para descifrar un mensaje\n"
                  << "3) Para salir\n";
        
        std::cout << "Su opción ha sido: ";
        std::cin >> menu;
        std::cout << "\n\n\n";

        if (menu == "1") Cifrar_Descifrar(true);
        else if (menu == "2") Cifrar_Descifrar(false);
        else if(menu == "3") break;
        else std::cout << "El valor introducido es incorrecto\n\n\n\n";
    }
}