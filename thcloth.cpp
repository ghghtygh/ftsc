
#include <vector>
#include <QtOpenGL>

#include "thcloth.h"
#include "canvas.h"

#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>


ThCloth::ThCloth(QObject *parent, Canvas *canvas)
	: QThread(parent), finish_bool(true), canvas(canvas)
{
	
}

void ThCloth::run()
{
	finish_bool = true;

	while (finish_bool) {

		canvas->cloth_updating();
		canvas->update_spring();

		msleep(50);
	}

	emit FinishCount(1);

}

void ThCloth::stop()
{

	finish_bool = false;

}
