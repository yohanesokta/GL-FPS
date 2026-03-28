#include "../loader.h"
#include "../globals.h"
#include "../wrapper/d3d.h"

namespace Magazine{
    class magazine
    {
    private:
        int ammoCount = 0;
        GLuint texture;
        float x,y;
    public:
        magazine(GLuint glTexture, int ammoCount, float x, float y);
        void draw();
    };
    
    magazine::magazine(GLuint glTexture, int ammoCount, float x, float y)
    {
            this->texture = glTexture;
            this->ammoCount = ammoCount;
            this->x = x;
            this->y = y;
    }

    void magazine::draw()
    {  
     
    }
}