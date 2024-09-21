#include <iostream>
#include <pthread.h>
#include <SFML/Graphics.hpp>

using std::cout, std::cin, std::endl;
using sf::RenderWindow, sf::VideoMode, sf::CircleShape, sf::RectangleShape, sf::Vector2f, sf::Event, sf::Color;

struct drawSeatParameters {
    RenderWindow* window;
    Color fillColor;
};

void* drawSeat(void* args){
    drawSeatParameters* params = static_cast<drawSeatParameters*>(args);
    RenderWindow* window = params->window;
    Color fillColor = params->fillColor;
    RectangleShape rectangle(Vector2f(25.f, 25.f));
    rectangle.setFillColor(fillColor);
    window->draw(rectangle);
    return NULL;
}

int main() {
    // Crear una ventana con un tamaño de 800x600 píxeles
    RenderWindow window(VideoMode(800, 800), "Segundo Examen Parcial Pablo Flores, Randy Rivera, Katherine Dieguez");

    drawSeatParameters params;

    params.window = &window;
    params.fillColor = Color::Red;

    pthread_t thread;
    pthread_create(&thread, NULL, drawSeat, &params);

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

        // Mostrar lo que se ha dibujado hasta ahora
        window.display();
    }

    pthread_join(thread, NULL);
    return 0;
}


