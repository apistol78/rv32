#include <Core/Log/Log.h>
#include <Core/Misc/SafeDestroy.h>
#include <Core/Misc/String.h>
#include <Ui/Application.h>
#include <Ui/Bitmap.h>
#include <Ui/Image.h>
#include <Ui/Form.h>
#include <Ui/FloodLayout.h>
#if defined(_WIN32)
#	include <Ui/Win32/WidgetFactoryWin32.h>
#elif defined(__APPLE__)
#	include <Ui/Cocoa/WidgetFactoryCocoa.h>
#elif defined(__LINUX__) || defined(__RPI__)
#	include <Ui/X11/WidgetFactoryX11.h>
#endif
#include "Rv32T/HDMI.h"
#include "Rv32T/LEDR.h"
#include "Rv32T/UART_TX.h"

// Verilated SoC
#include "SoC/VSoC.h"

using namespace traktor;

int main(int argc, char **argv)
{
#if defined(_WIN32)
	ui::Application::getInstance()->initialize(
		new ui::WidgetFactoryWin32(),
		nullptr
	);
#elif defined(__APPLE__)
	ui::Application::getInstance()->initialize(
		new ui::WidgetFactoryCocoa(),
		nullptr
	);
#elif defined(__LINUX__) || defined(__RPI__)
	ui::Application::getInstance()->initialize(
		new ui::WidgetFactoryX11(),
		nullptr
	);
#endif

	// Create user interface.
	Ref< ui::Form > form = new ui::Form();
	form->create(L"RV32", ui::dpi96(640), ui::dpi96(400), ui::Form::WsDefault, new ui::FloodLayout());

	Ref< ui::Bitmap > framebuffer = new ui::Bitmap(320, 200);
	
	Ref< ui::Image > image = new ui::Image();
	image->create(form, framebuffer, ui::Image::WsScale);

	form->update();
	form->show();

	// Create SoC simulation.
	VSoC* soc = new VSoC();
	soc->CPU_RESET_n = 1;

	HDMI hdmi;
	LEDR ledr;
	UART_TX uart_tx;

	for (;;)
	{
		if (!ui::Application::getInstance()->process())
			break;

		for (int32_t i = 0; i < 50000; ++i)
		{
			soc->CLOCK_125_p = 0;
			soc->eval();
			soc->CLOCK_125_p = 1;
			soc->eval();

			hdmi.eval(soc);
			ledr.eval(soc);
			uart_tx.eval(soc);
		}

		framebuffer->copyImage(hdmi.getImage());
		image->setImage(framebuffer);
	}

	safeDestroy(form);
	return 0;
}