#include <iostream>
#include <SFML/Graphics.hpp>

using std::cout, std::cin, std::endl;
using sf::RenderWindow, sf::VideoMode, sf::CircleShape, sf::Event;

int main() {
    // Crear una ventana con un tamaño de 800x600 píxeles
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ventana SFML");

    // Crear un círculo con un radio de 50 píxeles
    sf::CircleShape circle(50);
    // Establecer el color del círculo a verde
    circle.setFillColor(sf::Color::Green);

    // Bucle principal de la aplicación
    while (window.isOpen())
    {
        sf::Event event;
        // Procesar los eventos de la ventana (como cerrar)
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Limpiar la ventana con un color negro
        window.clear(sf::Color::Black);

        // Dibujar el círculo en la ventana
        window.draw(circle);

        // Mostrar lo que se ha dibujado hasta ahora
        window.display();
    }
    return 0;
}


