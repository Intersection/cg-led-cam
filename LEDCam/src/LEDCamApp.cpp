#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Surface.h"
#include "cinder/Capture.h"

#include "Resources.h"
#include "Constants.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LEDCamApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void resize( ResizeEvent event );
	void update();
	void draw();

	Capture					mCapture;
	gl::Texture				mTexture;
	gl::GlslProg			mShader;
	gl::Fbo					mFbo;

};

void LEDCamApp::prepareSettings( Settings *settings )
{
	settings->setFrameRate( kFrameRate );
	settings->setWindowSize( kWindowWidth, kWindowHeight );
}

void LEDCamApp::setup()
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
	
	mFbo = gl::Fbo( kWindowWidth, kWindowHeight );

}

void LEDCamApp::mouseDown( MouseEvent event )
{
}

void LEDCamApp::keyDown( KeyEvent event )
{
	if (event.getCode() == KeyEvent::KEY_f ){
		setFullScreen( !isFullScreen() );
	}
}

void LEDCamApp::resize( ResizeEvent event )
{
	mFbo = gl::Fbo( getWindowWidth(), getWindowHeight() );
}


void LEDCamApp::update()
{
	if( mCapture && mCapture.checkNewFrame() ) mTexture = gl::Texture( mCapture.getSurface() );
}

void LEDCamApp::draw()
{
	// clear out the window with black
	gl::clear( kClearColor ); 
	
	if( !mTexture ) return;
	mFbo.bindFramebuffer();
	mTexture.enableAndBind();
	mShader.bind();
	mShader.uniform( "tex", 0 );
	mShader.uniform( "bright", 0.3f );
	mShader.uniform( "ledScale", 100.0f );
	gl::drawSolidRect( getWindowBounds() );
	mTexture.unbind();
	mShader.unbind();
	mFbo.unbindFramebuffer();
	
	gl::Texture fboTexture = mFbo.getTexture();
	fboTexture.setFlipped();
	gl::draw( fboTexture );

}


CINDER_APP_BASIC( LEDCamApp, RendererGl(0) )
