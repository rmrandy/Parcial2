#include <iostream>
#include <pthread.h>
#include <SFML/Graphics.hpp>

using std::cout, std::cin, std::endl;
using sf::RenderWindow, sf::VideoMode, sf::CircleShape, sf::Event, sf::Color;

int main() {
    // Crear una ventana con un tamaño de 800x600 píxeles
    RenderWindow window(VideoMode(800, 800), "Segundo Examen Parcial Pablo Flores, Randy Rivera, Katherine Dieguez");

    // Crear un círculo con un radio de 50 píxeles
    CircleShape circle(50);
    // Establecer el color del círculo a verde
    circle.setFillColor(Color::Green);

    // Bucle principal de la aplicación
    while (window.isOpen())
    {
        Event event;
        // Procesar los eventos de la ventana (como cerrar)
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // Limpiar la ventana con un color negro
        window.clear(Color::Black);

        // Dibujar el círculo en la ventana
        window.draw(circle);

        // Mostrar lo que se ha dibujado hasta ahora
        window.display();
    }
    return 0;
}


