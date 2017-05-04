#ifndef SCENE_H
#define SCENE_H

#define RESOLUTION_WIDTH  640
#define RESOLUTION_HEIGHT 480

#include <stdio.h>
#include <stdlib.h>

// qt headers
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QVector>
#include <QSlider>
#include <QGLWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGLFunctions>

// opencv headers
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>

//#include <aruco/aruco.h>

//my headers
#include "principal.h"
#include "texture.h"

using namespace cv;
using namespace std;


class Skin : public QObject
{
    Q_OBJECT

public:

    int minimumHue;
    int maximumHue;
    int minimumSat;
    int maximumSat;
    int minimumVal;
    int maximumVal;

    Skin( QObject *parent = 0 ) : QObject( parent ),
                                  minimumHue( -1 ),
                                  maximumHue( -1 ),
                                  minimumSat( -1 ),
                                  maximumSat( -1 ),
                                  minimumVal( -1 ),
                                  maximumVal( -1 )
    {
    }

    void addMinHue( int hue )  {
        minimumHue = hue;
    }
    void addMaxHue( int hue )  {
        maximumHue = hue;
    }

    void addGoodValue( int hue, int sat, int val )
    {
        if( minimumHue < 0 || maximumHue < 0 )
        {
            minimumHue = hue; maximumHue = hue;
        }

        if( minimumSat < 0 || maximumSat < 0 )
        {
            minimumSat = sat; maximumSat = sat;
        }

        if( minimumVal < 0 || maximumVal < 0 )
        {
            minimumVal = val; maximumVal = val;
        }

        if( hue < minimumHue ) minimumHue = hue;
        if( hue > maximumHue ) maximumHue = hue;

        if( sat < minimumSat ) minimumSat = sat;
        if( sat > maximumSat ) maximumSat = sat;

        if( val < minimumVal ) minimumVal = val;
        if( val > maximumVal ) maximumVal = val;
    }
};



class Scene:  public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

    // VideoCapture para levantar la camara numero device
    int device;
    VideoCapture *videoCapture;

    // timer que determina cada cuando se obtiene un nuevo Mat
    QTimer * sceneTimer;

    // vector de clase propia que tiene Mat para cargar textura de la camara
    QVector< Texture * > *textures;

    // metodo principal, donde se procesara cada frame
    void process(Mat &frame);

    // punto donde esta el centro de masa.
    // es un punto tal, que cualquier recta que pasa por él,
    // divide a dicho segmento en dos partes de igual momento respecto a dicha recta
    Point baricenter;

    // vector para almacenar los puntos de los dedos (?)
    vector< Point > relevants;

    int textureIndex, modelIndex;
    int lastFingers;

public:

    Scene(QWidget *parent = 0);

    virtual ~Scene() {}

    // referencia de los colores de la piel
    Skin *refSkin;

    // getters and setters
    VideoCapture *getVideoCapture() const;
    void setVideoCapture(VideoCapture *value);

protected:

    // metodos de opengl
    void initializeGL();
    void resizeGL( int width, int height );
    void paintGL();


private:

    double distance( Point a, Point b );


private slots:

    void slot_updateScene();
};

#endif // SCENE_H
