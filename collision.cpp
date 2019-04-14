#include "collision.h"

#include "canvas.h"
#include "glcloth.h"

Collision::Collision(QObject *parent, Canvas *canvas)
	: QThread(parent), finish_bool(false), canvas(canvas)
{


}


void Collision::run()
{


	finish_bool = true;

	while (finish_bool) {

		set_vector();

		circle_collision();

		msleep(50);

	}

}
void Collision::circle_collision()
{
	


}
void Collision::set_vector()
{
	X0 = std::move(canvas->glc0->cl1->X);
	X1 = std::move(canvas->glc1->cl1->X);
	X2 = std::move(canvas->glc2->cl1->X);
	X3 = std::move(canvas->glc3->cl1->X);
	X4 = std::move(canvas->glc4->cl1->X);
	X5 = std::move(canvas->glc5->cl1->X);
}
void Collision::stop()
{
	finish_bool = false;
}