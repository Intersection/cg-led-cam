#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Surface.h"
#include "cinder/Capture.h"
#include "cinder/Rand.h"

#include "Resources.h"
#include "Constants.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LeCirqueApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void resize( ResizeEvent event );
	void update();
	void draw();

	Capture			mCapture;
	gl::Texture		mTexture;
	gl::GlslProg	mShader;
	gl::Fbo			mFbo;

	Surface			mSurface;
	Rand			mRand;
};

void LeCirqueApp::setup()
{
	try {
		mCapture = Capture( kCaptureWidth, kCaptureHeight );
		mCapture.start();
	} catch ( ... ) {
		console() << "Error with capture device." << std::endl;
		exit(1);
	}

	try {
		mShader = gl::GlslProg( loadResource( RES_SHADER_PASSTHRU ), loadResource( RES_SHADER_FRAGMENT ) );
	} catch ( gl::GlslProgCompileExc &exc ) {
		console() << "Cannot compile shader: " << exc.what() << std::endl;
		exit(1);
	}catch ( Exception &exc ){
		console() << "Cannot load shader: " << exc.what() << std::endl;
		exit(1);
	}
	
	mFbo = gl::Fbo( kCaptureWidth, kCaptureHeight );
	Rand::randomize();
}

void LeCirqueApp::mouseDown( MouseEvent event )
{
}

void LeCirqueApp::keyDown( KeyEvent event )
{
	if ( event.getCode() == KeyEvent::KEY_f ){
		setFullScreen( !isFullScreen() );
	}
}

void LeCirqueApp::resize( ResizeEvent event )
{
	mFbo = gl::Fbo( getWindowWidth(), getWindowHeight() );
}


void LeCirqueApp::update()
{
	if( mCapture && mCapture.checkNewFrame() ){
		mSurface = mCapture.getSurface();
	}
}

void LeCirqueApp::draw()
{
	// clear out the window with black
	gl::clear( kClearColor );

	if( !mSurface ) return;

	mFbo.bindFramebuffer();
	gl::clear( kClearColor );

	int gap = 1;
	Vec2f lastPoint = Vec2f::zero();
	PolyLine<Vec2f> line;
	gl::pushMatrices();
	Surface::Iter iter = mSurface.getIter();
	while( iter.line() ){
		line = PolyLine<Vec2f>();
		line.begin();

		while( iter.pixel() ){
			if( (iter.x() % gap) == 0 && (iter.y() % gap) == 0 ){
				gl::color( mSurface.getPixel( Vec2i( iter.x(), iter.y() ) ) );
				line.push_back( Vec2f( iter.x() , iter.y() ) );
			}
		}
		
		line.end();
		gl::draw( line );
	}

	gl::popMatrices();
	mFbo.unbindFramebuffer();

	gl::Texture fboTexture = mFbo.getTexture();
	fboTexture.setFlipped();
	mShader.bind();
	mShader.uniform( "tex", 0 );
	mShader.uniform( "mixColor", Vec3d( 0.0, 0.0, 0.0 ) );
	gl::draw( fboTexture );
	mShader.unbind();

}


CINDER_APP_BASIC( LeCirqueApp, RendererGl(2) )
