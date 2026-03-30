#include "loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../libs/stb_truetype.h"

float X_ROTATE[3] = {1.0f, 0.0f, 0.0f};
float Y_ROTATE[3] = {0.0f, 1.0f, 0.0f};
float Z_ROTATE[3] = {0.0f, 0.0, 1.0f};

GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 channels == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}

void drawTexturedQuad(GLuint textureID, float x, float y, float width, float height) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void draw3DQuad(GLuint textureID, float x, float y, float z, float size, float rotation[3], float rotationAngle) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotationAngle, rotation[0], rotation[1], rotation[2]);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-size/2, -size/2, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(size/2, -size/2, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(size/2, size/2, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-size/2, size/2, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

