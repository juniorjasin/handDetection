#include "scene.h"
#include <QDebug>

/*  proyecto: investigar opciones para controlar algo con la mano
 */


Scene::Scene(QWidget *parent): QGLWidget( parent ),
                               device( 1 ),
                               videoCapture ( new cv::VideoCapture( device ) ),
                               sceneTimer(new QTimer),
                               textures( new QVector< Texture * > ),
                               refSkin( new Skin( this ) )
{
    sceneTimer->start( 10 );
    connect( sceneTimer, SIGNAL( timeout() ), SLOT( slot_updateScene() ) );
}


void Scene::initializeGL()
{
    initializeGLFunctions();

    glClearColor( 0, 0, 0, 0 );
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );


    // no se que son estas cosas
    GLfloat lightAmbient[4]; lightAmbient[0] = 0.5f;  lightAmbient[1] = 0.5f;
            lightAmbient[2] = 0.5f;  lightAmbient[3] = 1.0f;

    GLfloat lightDiffuse[4]; lightDiffuse[0] = 1.0f;  lightDiffuse[1] = 1.0f;
            lightDiffuse[2] = 1.0f;  lightDiffuse[3] = 1.0f;

    GLfloat lightPosition[4];lightPosition[0]= 0.0f;  lightPosition[1]= 0.0f;
            lightPosition[2]= 2.0f;  lightPosition[3]= 1.0f;

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );  glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION,lightPosition ); glEnable( GL_LIGHT1 );

    textures->append( new Texture( "CameraTexture" ) );
}

void Scene::resizeGL( int width, int height )
{
    glViewport( 0, 0, width, height );
}

void Scene::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, RESOLUTION_WIDTH, 0, RESOLUTION_HEIGHT, 1, 1000 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Inicio: Gráfico de cámara
    glEnable( GL_TEXTURE_2D );
    glColor3f( 1, 1, 1 );
    glBindTexture( GL_TEXTURE_2D, textures->at( 0 )->id );
    glBegin( GL_QUADS );

        glTexCoord2f( 0, 0 ); glVertex3f( 0, RESOLUTION_HEIGHT, -999 );
        glTexCoord2f( 1, 0 ); glVertex3f( RESOLUTION_WIDTH, RESOLUTION_HEIGHT, -999 );
        glTexCoord2f( 1, 1 ); glVertex3f( RESOLUTION_WIDTH, 0, -999 );
        glTexCoord2f( 0, 1 ); glVertex3f( 0, 0, -999 );

    glEnd();
    glDisable( GL_TEXTURE_2D );
    // Fin: Gráfico de cámara


 /*
    // Fin: Gráfico de cámara

    glMatrixMode( GL_PROJECTION );
    double projectionMatrix[16];

    cv::Size2i sceneSize( RESOLUTION_WIDTH, RESOLUTION_HEIGHT );
    cv::Size2i openGlSize( RESOLUTION_WIDTH, RESOLUTION_HEIGHT );
    cameraParameters->glGetProjectionMatrix( sceneSize, openGlSize, projectionMatrix, 0.05, 10 );

    glLoadMatrixd( projectionMatrix );
    glMatrixMode( GL_MODELVIEW );
    double modelview_matrix[16];

    // Inicio: Graficos sobre la mano abierta
40
columnas Mat: 640
columnas Mat: 640
columnas Mat: 640
columnas Mat: 640
columnas Mat: 640
columnas Mat: 640
    if( matrix.size() == 12 && relevants.size() == 12 )
    {
        vector< Point2f > corners;
        corners.clear();

        corners.push_back( relevants.at( 10 ) );
        corners.push_back( relevants.at( 9 ) );
        corners.push_back( relevants.at( 2 ) );
        corners.push_back( relevants.at( 1 ) );

        Marker marker( corners, 1 );

        marker.calculateExtrinsicsHandMatrix( 0.08f,
                                              cameraParameters->CameraMatrix,
                                              matrix,
                                              cameraParameters->Distorsion,
                                              true );

        marker.glGetModelViewMatrix( modelview_matrix );
        glLoadMatrixd( modelview_matrix );

        // Dibuja imagenes planas
        glTranslatef( 0.005, y, z );
        glRotatef( rotacion, 1, 0, 0 );

//        drawSheet( ( textures->at( textureIndex )->name ), 35 );
        drawBox( ( textures->at( textureIndex )->name ), 20 );
//        drawModel( ( models->at( modelIndex )->name ), 8 );
//        drawVideo( "trailer-RF7.mp4", 100, 200 );

    }

    // Fin: Graficos sobre la mano abierta

    */

    glFlush();
}


/* slot llamado por la señal timeout de sceneTimer.
 * se obtiene cada nuevo frame y se lo pasa al metodo process
 * donde se procesara dicho frame
 */
void Scene::slot_updateScene()
{
    // verificamos si se pudo abrir la camara
    if(!this->getVideoCapture()->isOpened()){  // check if we succeeded
        qDebug() << "videocapture no se abrio correctamente";
        return;
    }

    videoCapture->operator >>( textures->operator []( 0 )->mat );
    process( textures->operator []( 0 )->mat );
    textures->operator []( 0 )->generateFromMat();
    this->updateGL();
}


/* se procesa cada frame de la camara o video
 * todo el trabajo esta en este metodo.
 */
void Scene::process(Mat &frame){

    // Filtramos por color

    Mat hsvFrame;
//    cvtColor( frame, hsvFrame, CV_BGR2HSV );  // Del Emi
    cvtColor( frame, hsvFrame, CV_BGR2Lab );  // Del Cesar

    std::vector< uint8_t > mask;

    for( int j = 0; j < hsvFrame.rows; j++ )
    {
        for( int i = 0; i < hsvFrame.cols; i++ )
        {
            Vec3b color = hsvFrame.at< Vec3b >( Point( i, j ) );

// Esto es del Emi que elige hue, sat y val haciendo clic en la pantalla
//            if( color[0] >= refSkin->minimumHue && color[0] <= refSkin->maximumHue &&
//                color[1] >= refSkin->minimumSat && color[1] <= refSkin->maximumSat &&
//                color[2] >= refSkin->minimumVal && color[2] <= refSkin->maximumVal )
//            {
//                mask.push_back( 255 );
//            }
//            else
//            {
//                mask.push_back( 0 );
//            }

            // Yo solo controlo el a del Lab sacado de los QSlider
            if( color[1] >= refSkin->minimumHue && color[1] <= refSkin->maximumHue )
            {
                mask.push_back( 0 );
            }
            else
            {
                mask.push_back( 255 );
            }

        }
    }

    Mat binary( Size( hsvFrame.cols, hsvFrame.rows ), CV_8UC1, ( void* )mask.data() );

    // Erosion y dilatacion de la imagen binaria

//    Mat matrix = ( Mat_< uchar >( 7, 7 ) << 0,0,1,1,1,0,0,
//                                            0,1,1,1,1,1,0,
//                                            1,1,1,1,1,1,1,
//                                            1,1,1,1,1,1,1,
//                                            1,1,1,1,1,1,1,
//                                            0,1,1,1,1,1,0,
//                                            0,0,1,1,1,0,0);

    int erosion_size = 9;
    Mat matrix = getStructuringElement( MORPH_CROSS,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );


    // erocion y dilatacion a la imagen binaria
    // @params (src, dst, kernel) para ambos
    erode( binary, binary, matrix );
    dilate( binary, binary, matrix );

    Mat binaryCopy = binary.clone();

    // Cierre convexo

    vector< Point > points;

    for( int i = 0; i < binary.rows; i++ )
    {
        for( int j = 0; j < binary.cols; j++ )
        {
            // si encuentra un punto blanco pertenece al cierre convexo ?
            if( ( ( int )binary.at< uchar >( i, j ) ) == 255 )
            {
                points.push_back( Point( j, i ) );
            }
        }
    }

    vector< Point > hull;
    hull.clear();

    convexHull( Mat( points ), hull, false );

    // Dibujamos extremos

    int minimumDistance = 3000;

    vector< Point > baricenters;

    if( hull.size() > 1 )
    {
        if( distance( hull.at( 0 ), hull.at( ( hull.size() - 1 ) ) ) > minimumDistance )
        {
            baricenters.push_back( hull.at( 0 ) );
        }
    }

    for( unsigned int i = 1; i < hull.size(); i++ )
    {
        if( distance( hull.at( i ), hull.at( i - 1 ) ) > minimumDistance )
        {
            baricenters.push_back( hull.at( i ) );
        }
    }

    // Centro de masa segun cierre convexo

    if( baricenters.size() > 0 )
    {
        baricenter.x = 0;
        baricenter.y = 0;

        for( unsigned int i = 0; i < baricenters.size(); i++ )
        {
            baricenter.x += baricenters.at( i ).x;
            baricenter.y += baricenters.at( i ).y;
        }

        baricenter.x /= baricenters.size();
        baricenter.y /= baricenters.size();

        // Dibuja el centro de la palma
//        circle( frame, baricenter, 10 , Scalar( 255, 0, 255 ), 2 );

        // Dibuja lineas grises desde el centro a los bordes
        for( unsigned int i = 0; i < baricenters.size(); i++ )
        {
            line( frame, baricenters.at( i ), baricenter, Scalar( 128, 128, 128 ), 1 );
        }
    }

    // Dibujamos bordes, Entre las puntas de los dedos
    if( hull.size() > 1 )
        line( frame, hull.at( 0 ), hull.at( ( hull.size() - 1 ) ), Scalar( 128, 128, 128 ), 1 );

    double distanciaEuclidian = 0;
    double sumatoriaDistancia = 0;
    double sumatoriaVarianzaDistancia = 0;
    float promediaDistancia = 0;
    float varianzaDistancia = 0;

    // Dibujamos bordes, Entre las puntas de los dedos
    for( unsigned int i = 1; i < hull.size(); i++ )
    {
        line( frame, hull.at( i ), hull.at( i - 1 ), Scalar( 128, 128, 128 ), 1 );

        distanciaEuclidian = cv::norm(hull.at( i ) - hull.at( i - 1 ));//Euclidian distance
        sumatoriaDistancia += distanciaEuclidian;
//        qDebug() << "distanciaEuclidian=" << distanciaEuclidian;

    }

    promediaDistancia = sumatoriaDistancia/(hull.size()-1);
//    qDebug() << "promediaDistancia=" << promediaDistancia;

    for( unsigned int i = 1; i < hull.size(); i++ )
    {

        distanciaEuclidian = cv::norm(hull.at( i ) - hull.at( i - 1 ));//Euclidian distance
        sumatoriaVarianzaDistancia = sumatoriaVarianzaDistancia +
                (distanciaEuclidian-promediaDistancia) * (distanciaEuclidian-promediaDistancia);
//        qDebug() << "sumatoriaVarianzaDistancia=" << sumatoriaVarianzaDistancia;

    }

    varianzaDistancia = sqrt(sumatoriaVarianzaDistancia/(hull.size()-1));
    qDebug() << "varianzaDistancia = " << varianzaDistancia;


    // Buscamos los contornos en la imagen binaria
    vector< vector< Point > > contours;
    findContours( binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

    relevants.clear();
    int fingers = 1;

    for( unsigned int i = 0 ; i < contours.size(); i++ )
    {
        // Ignoramos las areas insignificantes
        if( contourArea( contours[i] ) >= 3000 )
        {
            // Detectamos cierre convexo en el contorno actual
            vector<vector< Point > > hulls( 1 );
            vector<vector< int > > hullsI( 1 );
            convexHull( Mat( contours[i] ), hulls[0], false );
            convexHull( Mat( contours[i] ), hullsI[0], false );

            // Buscamos defectos de convexidad
            vector< Vec4i > defects;
            if ( hullsI[0].size() > 0 )
            {
                convexityDefects( contours[i], hullsI[0], defects );
                if( defects.size() >= 3 )
                {
                    qDebug() << "\n==============================";

                    float umbral = 0;  // umbral para la profundidad de la concavidad
                    float sumatoria = 0;
                    float promedio = 0;
                    float varianza = 0;

                    // Para el promedio
                    for( unsigned int j = 0; j < defects.size(); j++ )
                    {
                        float depth = defects[j][3] / 256;

                        sumatoria += depth;
                    }
                    promedio = sumatoria/defects.size();
//                    qDebug() << "Promedio = " << promedio;

                    sumatoria = 0;  // Ponemos en cero para usarla para la varianza

                    // Para la varianza
                    for( unsigned int j = 0; j < defects.size(); j++ )
                    {
                        float depth = defects[j][3] / 256;
                        sumatoria = sumatoria + (depth-promedio) * (depth-promedio);
                    }

                    varianza = sqrt(sumatoria/defects.size());
//                    qDebug() << "Varianza = " << varianza;

                    for( unsigned int j = 0; j < defects.size(); j++ )
                    {
                        // the farthest from the convex hull point within the defect
                        float depth = defects[j][3] / 256;



//                        qDebug() << depth;

                        // Filtramos por profundidad
//                        if( depth > 100 )  // Umbral para que no detecte valle en la muneca
//                        if( depth > 70 )
                        if( depth > promedio )
                        {
                            // Entra a este if cada vez que supere esta depth, deberia detectar 4 depth que superen
                            // este umbral. Es decir, detectar los 4 valles. Si el antebrazo se muestra mucho,
                            // entonces quizas se detecte un valle mas, que parezca un sexto dedo.

                            // Cuando la mano se aleja, los depth no llegan a superar el umbral. Deberia fijarse
                            // como umbral un porcentaje y no una distancia fija.

                            // Valles = convexity defects = defectos de convexidad
                            // Envoltura convexa = convex hull

                            int startidx = defects[j][0];
                            Point ptStart( contours[i][startidx] );

                            int endidx = defects[j][1];
                            Point ptEnd( contours[i][endidx] );

                            int faridx = defects[j][2];
                            Point ptFar( contours[i][faridx] );

//                            qDebug() << "cv::norm(ptStart - ptEnd)"  << cv::norm(ptStart - ptEnd);


//                            if ( cv::norm(ptStart - ptEnd) < varianzaDistancia )  {

                                circle( frame, ptStart, 8, Scalar( 255, 0, 0 ), 2 );
                                circle( frame, ptFar, 8, Scalar( 0, 255, 0 ), 2 );
                                circle( frame, ptEnd, 8, Scalar( 0, 0, 255 ), 2 );
//                            }

                            relevants.push_back( ptStart );
                            relevants.push_back( ptFar );
                            relevants.push_back( ptEnd );

                            fingers++;
                        }
                    }
                }
            }
        }
    }

    if( fingers > 1 )
    {
        QString text( "Dedos: " + QString::number( fingers ) );
        putText( frame, text.toStdString(), Point( 10, 30 ), 1, 2, Scalar( 255, 0, 0 ) );
    }

    // Aca se detecta la interaccion para cambiar de modelo a dibujar
    if( fingers == 5  && lastFingers == 4 )
    {
        textureIndex++;
        modelIndex++;

        if( textureIndex >= textures->size() ) textureIndex = 0;
        //if( modelIndex >= models->size() ) modelIndex = 0;
    }

    lastFingers = fingers;

    // Mostramos miniatura
    Mat preview( binaryCopy.rows, binaryCopy.cols, CV_8UC3 );
    cvtColor( binaryCopy, preview, CV_GRAY2BGR );
    Mat previewResized( 96, 128, CV_8UC3 );
    cv::resize( preview, previewResized, previewResized.size(), 0, 0, INTER_CUBIC );
    previewResized.copyTo( frame( Rect( frame.cols - 135, frame.rows - 103, 128, 96 ) ) );

}


double Scene::distance( Point a, Point b )
{
    return ( a.x - b.x ) * ( a.x - b.x ) + ( a.y - b.y ) * ( a.y - b.y );
}



//----------------------------getters and setters---------------------------------
VideoCapture *Scene::getVideoCapture() const
{
    return videoCapture;
}

void Scene::setVideoCapture(VideoCapture *value)
{
    videoCapture = value;
}



