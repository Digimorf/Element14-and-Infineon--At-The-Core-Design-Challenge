<div class="layout-region-inner content">

<div id="fragment-7293"
class="content-fragment blog-title no-wrapper responsive-1"
reflow="_p_content,_p_singlecolumn,1,1,7">

<div class="content-fragment-content">

# "ATCDC: Digimorf, SEGA SG-1000 Emulator on PSoC62S4 Pioneer kit, everything unveiled!

</div>

<div class="content-fragment-footer">

</div>

</div>

<div id="fragment-7294">

</div>

<div id="fragment-7295"
class="content-fragment blog-post-byline no-wrapper with-spacing responsive-1"
reflow="_p_content,_p_singlecolumn,1,1,9">

<div class="content-fragment-content">

<div class="content-details">

<div class="content-author">

<div class="avatar">

[<img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/24/55/56/4UJ9DW4IF7NM.jpg-80x80x2.jpg?_=pH9y4DBcdmY+TkdI0iRaXw==" data-border="0px" alt="Digimorf" />](/members/digimorf)

</div>

<span class="user-name">
<a href="/members/digimorf" class="internal-link view-user-profile">Digimorf</a>
</span>

</div>

<div class="content-date">

5 May 2023

</div>

</div>

</div>

<div class="content-fragment-footer">

</div>

</div>

<div id="fragment-7296"
class="content-fragment blog-post-poster no-wrapper with-spacing responsive-1"
reflow="_p_content,_p_singlecolumn,1,1,10">

<div class="content-fragment-content">

<div class="content full text">

<div class="poster">

![SEGA SG-1000 Emulator on PSoC62S4 Pioneer
kit](https://community-storage.element14.com/communityserver-components-secureimagefileviewer/communityserver/blogs/components/weblogfiles/00/00/00/03/92/2335.Header.jpg-1440x400x2.jpg?sv=2016-05-31&sr=b&sig=3BFLNlTmUQElDTICPq2tPS5CSQzkVX26Ro2V594stxY%3D&se=2023-07-20T23%3A59%3A59Z&sp=r&_=0tCQSh+Vpjrzb5r+BEXNfA==)

</div>

</div>

</div>

<div class="content-fragment-footer">

</div>

</div>

<div id="fragment-7297"
class="content-fragment blog-post no-wrapper with-spacing responsive-1"
reflow="_p_content,_p_singlecolumn,1,1,11">

<div class="content-fragment-content">

<div class="content full text">

<div class="content">

<span style="font-size:150%;">**SEGA SG-1000 Emulator on PSoC62S4
Pioneer kit**</span>

<span style="font-size:inherit;">Hi everyone!</span>

<span style="font-size:inherit;">Here we are at the conclusion of this
adventure in the world of Infineon with my project for this
challenge.</span>

<span style="font-size:inherit;">In these two months, I have tried to
share all the salient steps in the creation of a SEGA SG-1000 console
emulator. Since the purpose of this challenge was to push this system to
its limits, I felt it was important to share the steps taken by a
developer using a PSoC6 for the first time for the realization of a
multi-disciplinary project: generation of a video signal, generation of
audio in real-time, implementation of a virtual machine, all trying to
use the resources (limited for this type of project) of this
microcontroller.</span>

<span style="font-size:inherit;">This final post wants to officially be
a more exhaustive description of this project.</span>

<span style="font-size:150%;">**1.0 - Introduction**</span>

<img src="/resized-image/__size/658x318/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/variation_5F00_5F00_5F00_img_5F00_5F00_5F00_2.jpg" class="align-right" style="float: right; max-height: 159px; max-width: 329px; cursor: zoom-in;" alt="SEGA SG-1000" />The
idea for this project came to me while looking at this PSoC62 Pioneer
Kit. On the PCB it features a "Touchpad" which reminded me of a Joypad.
The challenge launched by Element14 together with Infineon proposed
reaching the limits of this system, trying to exploit the
characteristics of the on-board microcontroller.

As I explained in my posts during this challenge, I'm a retro enthusiast
and have written several emulators in the past. My favorite system,
which was the first one I had in my youth, is the SEGA SG-1000 console.
So I wanted to rewrite one of the SEGA SG-1000 emulators that I have
written and port it to the board in question.

<span style="font-size:150%;">**1.1 - Difficulties and limits**</span>

<span
style="font-size:inherit;"><img src="/resized-image/__size/480x720/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/6661.Sega_5F00_5F00_5F00_SG_5F00_2D00_5F00_1000_5F00_2D00_5F00_v1_5F00_2D00_5F00_PCB.jpg" class="align-right" loading="lazy" style="float: right; max-height: 360px; max-width: 240px; cursor: zoom-in;" alt="SEGA SG-100 mainbord" />Emulating
a game console is a rather complex, resource-intensive project. I
thought it was a perfect candidate to push the PSoC62 Pioneer Kit to the
limit.</span>

<span style="font-size:inherit;">I can say that an emulator is divided
into two main areas: Virtual Machine and Hardware Input/output. The
microcontroller on board is a dual-core Arm Cortex M4/M0+, so I can
divide these areas into one core each. </span>

<span style="font-size:inherit;">The two cores are configurable through
the ModusToolbox ecosystem, so it is possible to decide how to divide
the resources between the two cores, and at what speed to set the
clocks. For me, it was a nice surprise to see the high customization and
ease of use of ModusToolbox.</span>

<span style="font-size:inherit;">In the end, I decided to set the CM4
core at 150MHz and the CM0+ at 75MHz. I left the emulation of the
Input/Output hardware components at the CM0+ core, and the execution of
the virtual machine at the CM4 core. That means that the cm0+ will
generate the VGA video and the audio signals, it will handle also the
Capsense joypad, and it will synchronize the execution of the virtual
machine in the CM4 core.</span>

<span
style="font-size:inherit;"><img src="/resized-image/__size/480x720/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/7206.CY8CKIT.jpg" class="align-left" loading="lazy" style="float: left; max-height: 360px; max-width: 240px; cursor: zoom-in;" alt="CY8CKIT-062S4 PSoC 62S4 pioneer kit" />Another
aspect that is difficult to manage is the division of the SRAM between
the two cores. The original hardware to be emulated has 2Kb of RAM, 16Kb
of special VRAM used by the Video Display Processor, and last but not
least, the memory used by the game cartridge.</span>

The Infineon PSoC 6 MCU in the Pioneer Kit is the CY8C6244LQI-S4D92,
which has 128Kb SRAM and 256Kb flash ROM. Apparently, everything should
work perfectly, but it is not so obvious. The microcontroller must
generate a video signal, which for the console to be emulated, must have
a resolution of 256x192 pixels, therefore, unless it were possible to
generate all the graphics on the fly, a frame buffer of about 49Kb is
needed to host a video frame.

Then, in the same way, the audio generation relies on a buffer that
contains all the samples to stream. in this case, we need one of around
1Kb (more on this later). Don't forget that the source code of the
emulator will create variables, temporary buffers, etc. in a stack, so
you need to give a reasonable space to stack and heap.

<img src="/resized-image/__size/662x446/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/3000.VGAtimings.png" class="align-right" loading="lazy" style="float: right; height: 223px; max-height: 223px; max-width: 331px; cursor: zoom-in;" width="330" height="223" alt="VGA timings" />Speed
is also crucial, since the cm0+ core must be able to generate a standard
VGA signal, and the CM4 core must be able to perform a number of
calculations to emulate a 3.58MHz CPU, a 10.74MHz VDP, and a
Programmable Sound Generator (PSG) at 3.58MHz, in order to display
graphics, sound and manage a Joypad in real-time.

<span style="font-size:150%;"></span>Two months were available for this
project, so I had to accept a compromise between the quality and
usability of the emulator. 

<span style="font-size:150%;">**1.2 - The emulator and the game**</span>

<span style="font-size:inherit;">Two months were available for this
project, so I had to accept a compromise between the quality and
usability of the emulator. </span>

<span style="font-size:inherit;">The emulator will have to perform
different tasks divided into time intervals so that it is possible to
simulate hardware composed of several units that work in multitasking.
Since the emulator is synchronized with a video signal, the time
intervals will be equal to a video scanline. Then, the Z80 virtual CPU
will run for a series of cycles equal to the time interval of an
emulated video scanline, and then the graphics and sound for that
scanline will be rendered.</span>

<span style="font-size:inherit;">I won't go too much into the details of
the emulation, but this means that c</span><span
style="font-size:inherit;">onsidering the VGA standard timings:</span>

## Horizontal timing (line)

<span style="font-size:inherit;">Polarity of horizontal sync pulse is
negative.</span>

<div
class="content-scrollable-wrapper content-scrollable-wrapper-scrolled"
style="max-width: 100%; overflow: auto; max-height: 668px;">

|               |        |                  |
|---------------|--------|------------------|
| Scanline part | Pixels | Time \[µs\]      |
| Visible area  | 640    | 25.422045680238  |
| Front porch   | 16     | 0.63555114200596 |
| Sync pulse    | 96     | 3.8133068520357  |
| Back porch    | 48     | 1.9066534260179  |
| Whole line    | 800    | 31.777557100298  |

</div>

## Vertical timing (frame)

<span style="font-size:inherit;">Polarity of vertical sync pulse is
negative.</span>

<div
class="content-scrollable-wrapper content-scrollable-wrapper-scrolled"
style="max-width: 100%; overflow: auto; max-height: 668px;">

|              |       |                   |
|--------------|-------|-------------------|
| Frame part   | Lines | Time \[ms\]       |
| Visible area | 480   | 15.253227408143   |
| Front porch  | 10    | 0.31777557100298  |
| Sync pulse   | 2     | 0.063555114200596 |
| Back porch   | 33    | 1.0486593843098   |
| Whole frame  | 525   | 16.683217477656   |

</div>

<span style="font-size:inherit;">I need to render a complete scanline
(CPU + VDP + PSG + IO) in 31.777 us. It may seem like an impossible
mission, but there are tricks that we can use to do it. So this should
be the final result of the game that I want to run on this
emulator:</span>

<span style="font-size:inherit;"> </span>

<div
class="se-vm-wrapper fragment-b3ddff4f373f41a3befafb182a78f9e91547470482_media se-video-view se-video-frame-container"
style="width: 640px; height: 389px; display: inline-block;">

<div
class="se-video-viewer-flex-space-between se-video-viewer-controls">

<div>

<span class="se-media-rating-control rating"
style="white-space: nowrap;"><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a></span>

</div>

<div class="navigation-list" direction="vertical">

<span class="se-video-share-open se-video-button">Share</span>

</div>

</div>

<div class="se-video-modal-curtain se-modal-enabled hidden">

</div>

<div class="se-video-modal se-modal-enabled hidden">

<div class="modal-title">

<div class="modal-close-wrapper">

<div class="se-video-modal-close modal-close">

</div>

</div>

<div style="pointer-events: none; user-select: none;">

Share media

</div>

</div>

<div class="se-video-viewer-flex-space-between"
style="margin: 20px 0 10px 0;">

<div class="navigation-list" direction="vertical">

<span class="se-video-share-copy se-video-button">COPY</span>

</div>

</div>

</div>

</div>

<div
class="se-vm-wrapper fragment-b3ddff4f373f41a3befafb182a78f9e91547470482_media se-video-editor hidden">

<div class="se-video-modal-curtain hidden">

</div>

<div class="se-video-modal">

<div class="modal-title hidden">

<div class="modal-close-wrapper">

<div class="se-video-modal-close modal-close">

</div>

</div>

<div style="pointer-events: none; user-select: none;">

Edit media

</div>

</div>

<div>

<div class="se-video-flex-row-center se-video-editor-dimensions hidden"
style="margin-bottom: 20px;">

Dimensions <span style="margin: 0 5px;">x</span> Small Medium Large
Custom

</div>

Subject **(required)** Brief Description Tags (separated by comma) Video
visibility in search results Visible Hidden

<div>

Parent content

<div class="se-video-flex-row"
style="align-items: center; margin-bottom: 15px;">

ATCDC: Digimorf, Day 3, The project SEGA SG-1000 Emulator on PSoC62S4
Pioneer kit! "ATCDC: Digimorf, SEGA SG-1000 Emulator on PSoC62S4 Pioneer
kit, everything unveiled!

</div>

</div>

Poster

<div class="se-video-flex-row" style="margin-bottom: 10px;">

<span
class="se-video-editor-og-image-upload se-video-editor-anchor">Upload</span>
<span
class="se-video-editor-og-image-preview-ext se-video-editor-anchor">Preview</span>

</div>

<div style="height: 100px">

<div class="se-video-editor-og-image-wrapper hidden">

</div>

</div>

<div style="margin-top: 20px;">

<div
class="se-editor-submit-button mce-widget mce-btn mce-primary mce-first mce-btn-has-text"
tabindex="-1" role="button">

<span class="mce-txt">Ok</span>

</div>

<div
class="se-editor-cancel-button mce-widget mce-btn mce-last mce-btn-has-text"
tabindex="-1" role="button">

<span class="mce-txt">Cancel</span>

</div>

</div>

</div>

</div>

</div>

<span style="font-size:inherit;">And this is the same game emulated by
the PSoC62S4 Pioneer kit:</span>

<span style="font-size:inherit;"> </span>

<div
class="se-vm-wrapper fragment-b3ddff4f373f41a3befafb182a78f9e91363408592_media se-video-view se-video-frame-container"
style="width: 854px; height: 389px; display: inline-block;">

<div
class="se-video-viewer-flex-space-between se-video-viewer-controls">

<div>

<span class="se-media-rating-control rating"
style="white-space: nowrap;"><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a><a href="#" class="ui-tip"><span class="star-off-left"></span></a><a href="#" class="ui-tip"><span class="star-off-right"></span></a></span>

</div>

<div class="navigation-list" direction="vertical">

<span class="se-video-share-open se-video-button">Share</span>

</div>

</div>

<div class="se-video-modal-curtain se-modal-enabled hidden">

</div>

<div class="se-video-modal se-modal-enabled hidden">

<div class="modal-title">

<div class="modal-close-wrapper">

<div class="se-video-modal-close modal-close">

</div>

</div>

<div style="pointer-events: none; user-select: none;">

Share media

</div>

</div>

<div class="se-video-viewer-flex-space-between"
style="margin: 20px 0 10px 0;">

<div class="navigation-list" direction="vertical">

<span class="se-video-share-copy se-video-button">COPY</span>

</div>

</div>

</div>

</div>

<div
class="se-vm-wrapper fragment-b3ddff4f373f41a3befafb182a78f9e91363408592_media se-video-editor hidden">

<div class="se-video-modal-curtain hidden">

</div>

<div class="se-video-modal">

<div class="modal-title hidden">

<div class="modal-close-wrapper">

<div class="se-video-modal-close modal-close">

</div>

</div>

<div style="pointer-events: none; user-select: none;">

Edit media

</div>

</div>

<div>

<div class="se-video-flex-row-center se-video-editor-dimensions hidden"
style="margin-bottom: 20px;">

Dimensions <span style="margin: 0 5px;">x</span> Small Medium Large
Custom

</div>

Subject **(required)** Brief Description Tags (separated by comma) Video
visibility in search results Visible Hidden

<div>

Parent content

<div class="se-video-flex-row"
style="align-items: center; margin-bottom: 15px;">

ATCDC: Digimorf, Day 60, The project SEGA SG-1000 Emulator on PSoC62S4
Pioneer kit, part 13 "ATCDC: Digimorf, SEGA SG-1000 Emulator on PSoC62S4
Pioneer kit, everything unveiled!

</div>

</div>

Poster

<div class="se-video-flex-row" style="margin-bottom: 10px;">

<span
class="se-video-editor-og-image-upload se-video-editor-anchor">Upload</span>
<span
class="se-video-editor-og-image-preview-ext se-video-editor-anchor">Preview</span>

</div>

<div style="height: 100px">

<div class="se-video-editor-og-image-wrapper hidden">

</div>

</div>

<div style="margin-top: 20px;">

<div
class="se-editor-submit-button mce-widget mce-btn mce-primary mce-first mce-btn-has-text"
tabindex="-1" role="button">

<span class="mce-txt">Ok</span>

</div>

<div
class="se-editor-cancel-button mce-widget mce-btn mce-last mce-btn-has-text"
tabindex="-1" role="button">

<span class="mce-txt">Cancel</span>

</div>

</div>

</div>

</div>

</div>

<span style="font-size:inherit;">I decided to use "Space Invaders" for
one reason only: The Capsense area present on the Pioneer Kit has 1 axis
direction slider and two buttons, which is perfect for this game. I used
the slider as a rotary encoder, and the two buttons as normal fire
buttons.</span>

<span style="font-size:150%;">**2.0 - Architecture**</span>

<span style="font-size:inherit;">Let's examine the architecture of this
project in the following schematic:</span>

<span
style="font-size:inherit;"><img src="/resized-image/__size/2064x986/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/Application.png" loading="lazy" style="display: block; height: 493px; margin-left: auto; margin-right: auto; max-height: 493px; max-width: 1032px; cursor: zoom-in;" width="1031" height="426" alt="SEGA SG-1000 emulator on PSoC62 diagram" /></span>

<span style="font-size:inherit;">This schematic shows clearly that the
hardware interface of the emulator is handled by the CM0+ core, while
the software emulation of the console is left to the CM4 core. In
ModusToolbox the system is configured for using the external 24MHz clock
on the Pioneer Kit that offers a better accuracy and stability of the
operating frequencies for the cores.</span>

<span
style="font-size:inherit;"><img src="/resized-image/__size/1850x1178/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/8304.clocking_2D00_system.jpg" loading="lazy" style="display: block; height: 589px; margin-left: auto; margin-right: auto; max-height: 589px; max-width: 925px; cursor: zoom-in;" width="925" height="530" alt="image" /></span>

<span style="font-size:inherit;">The routing shown in the system
schematic of the ModusToolbox Device Configurator sets the clock for the
CM0+ core to 75MHz, and the clock of the CM4 to 150MHz. So the minimum
time interval of the Peripheral Clock is 13.33 ns, enough for the timers
and the DMA functionality.</span>

<span style="font-size:150%;">**2.1 - Hardware side on CM0+**</span>

<span style="font-size:inherit;">The CM0+ core </span><span
style="font-size:inherit;">takes care of driving the hardware interfaces
for the emulator, such as Video, Audio, and I/O. So I started first with
the development of the VGA video driver since it's the unit that rules
all the emulation.</span>

<span style="font-size:150%;">**2.1.1 - VGA DAC**</span>

As I mentioned earlier, the video signal generated complies with the VGA
standard, therefore with a vertical frequency of 60Hz and a horizontal
one of 31.468 kHz. The vertical resolution maintains the standard of 525
total lines for a visible vertical area of 480 lines, while for the
horizontal resolution, we will use a "Pixel Clock" which we will
configure to obtain a resolution suitable for the emulation.

The Pioneer Kit does not have a VGA video output, but building it is
very simple and requires a few resistors, a DE15 socket connector, some
jumper wires, and a solderless breadboard. All components can be easily
found on eBay or Amazon, but I wanted to bring your attention to this
solderless VGA socket:

[Solderless VGA
plug](https://www.amazon.com/ANMBEST-Solderless-Connector-Terminal-Compatible/dp/B08YJKG1HV/ref=sr_1_4?crid=320HUR1FEDR0V&keywords=vga%2Bplug&qid=1683023856&sprefix=vga%2Bplu%2Caps%2C176&sr=8-4&th=1 "Solderless VGA plug")

<img src="/resized-image/__size/1304x1038/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/8132.RGBDAC.png" loading="lazy" style="display: block; height: 519px; margin-left: auto; margin-right: auto; max-height: 519px; max-width: 652px; cursor: zoom-in;" width="651" height="519" alt="Solderless breadboard RGB VGA DAC" />

This little circuit is a triple analog-to-digital converter that takes
the values of Red, Green and Blue and converts them into voltages using
simple resistors. The proposed circuit offers a resolution of 8 shades
of Red and Green, while only 4 for Blue, which however are sufficient
for the visual impact. This division is called "color depth" and is
usually referred to as "BPP" or bits per plane.  
The signal will therefore be rgb332 and will allow us to obtain 256
colors and the following palette.

<img src="/resized-image/__size/876x494/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/8053.palette_2D00_rgb332.png" loading="lazy" style="display: block; margin-left: auto; margin-right: auto; max-height: 247px; max-width: 438px; cursor: zoom-in;" alt="image" />

This color depth is very useful because it fits right into a byte,
making it easy to match 1 pixel to 1 byte. Here we understand why to
match an entire GPIO port to the used RGB signals.

<img src="/resized-image/__size/1200x1064/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/4812.PioneerKitVGA.png" loading="lazy" style="display: block; margin-left: auto; margin-right: auto; max-height: 532px; max-width: 600px; cursor: zoom-in;" alt="image" />

<span style="font-size:150%;">**2.1.2 - Audio Signal**</span>

The audio signal is generated by the DAC embedded into the PSoC62, and
it is fed by the virtual PSG of the emulator. The simplest hardware you
need to plug your Pioneer Kit is a bipolar shielded wire for audio
signals and a 3.5mm stereo jack plug.

<img src="/resized-image/__size/906x386/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/5483.audio.png" loading="lazy" style="display: block; height: 193px; margin-left: auto; margin-right: auto; max-height: 193px; max-width: 453px; cursor: zoom-in;" width="452" height="96" alt="image" />

The resolution of this DAC is 12 bits, this means that the signal coming
out of the assigned GPIO can assume 2^12 values to modulate a voltage
between 0 and 3.3v (typically).

<img src="/resized-image/__size/1200x1064/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/1263.PioneerKitAudio.png" loading="lazy" style="display: block; margin-left: auto; margin-right: auto; max-height: 532px; max-width: 600px; cursor: zoom-in;" alt="image" />

<span style="font-size:150%;">**2.1.3 - Capsense Joypad**</span>

The Pioneer Kit has some capacitive areas on board that are connected to
specific pins of the PSoC62. These pins can be dri0ven with the Capsense
technology which simplifies the configuration and the building of
capacitive devices. In this specific project, this interface reminds us
of a one-axis joypad with two fire buttons. So, it's possible to use it
directly as an input device by configuring it through the Capsense
configurator.

<img src="/resized-image/__size/1930x762/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/7120.CapsenseJoypad.png" loading="lazy" style="display: block; height: 381px; margin-left: auto; margin-right: auto; max-height: 381px; max-width: 965px; cursor: zoom-in;" width="964" height="348" alt="image" />

<span style="font-size:150%;">**2.2 - Software side on CM0+**</span>

The project was derived from the "Dual-CPU Empty PSoC6 App" example
project within ModusToolbox. As shown above, I modified the clock
system, and the Board Support Package, and from there I added various
video, audio, and joypad devices.

<span style="font-size:150%;">**2.2.1 - VGA Driver**</span>

The VGA driver is capable of generating a video signal that complies
with the VGA standard, so it should be compatible with most TV/Monitors
that can connect to a PC.  
The generated signal consists of the generation of 60 frames per second
composed of a sequence of 525 scanlines. The scanlines are divided into
groups or areas: Vertical Sync, the Back Porch which precedes the actual
image, the Active Area with the image made up of 480 lines of pixels,
and finally the Front Porch which closes the image.

<img src="/resized-image/__size/1718x1930/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/2352.VGA_5F00_VideoDriver_5F00_Frame.png" loading="lazy" style="display: block; height: 965px; margin-left: auto; margin-right: auto; max-height: 965px; max-width: 859px; cursor: zoom-in;" width="858" height="965" alt="image" />For
more details, you can check my posts on this blog, but here I can say
that all the scanlines are composed of the two sync signals (Horizontal
and Vertical), ad the three color signals (R, G, B). In all scanlines,
there is a horizontal sync pulse, and the vertical sync signal is high,
except for the vertical sync period, which presents two lines with a low
state for the vertical sync. 

The color signals are at a blank level (black) in all scanlines except
in the Active Area, where they are modulated thanks to the data coming
from the frame buffer and combined together by the resistor DAC.

To send the correct line, the driver counts the scanlines within an
interrupt service routine (ISR), which is triggered at the end of each
line via an IRQ event. In order to execute a line with the correct
characteristics, a Counter with a PWM signal is used, which integrates
an impulse, in this case negative, corresponding to the horizontal
synchronism of the standard VGA signal. In addition, two "Counter
Compare" events are set along the line to activate the RGB signals and
therefore the streaming of the pixels at regular intervals, according to
a "Pixel Counter".

We have seen that the graphics resolution of the SEGA SG-1000 is
256x192, and the resolution for the VGA signal is 640x480. We can double
the original resolution and get a bigger picture. For horizontal
resolution, we just need to "slow down" the PIXEL Clock, while for
vertical resolution we can transmit the same line twice.

We have seen that the graphics resolution of the SEGA SG-1000 is
256x192, and the resolution for the VGA signal is 640x480. We can double
the original resolution and get a bigger picture. For horizontal
resolution, we just need to "slow down" the PIXEL Clock, while for
vertical resolution we can transmit the same line twice. Since we are
emulating a console that typically used to be hooked up to a CRT
television, we can transmit one line, and turn off the next line to
create the typical effect of scanlines. We also need this trick to free
up access to the frame buffer in RAM while the CM4 core accesses it
during the rendering phase.

Let's examine how this system works:

<img src="/resized-image/__size/1896x1126/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/7142.VGA_5F00_VideoDriver_5F00_Scanline.png" loading="lazy" style="display: block; height: 563px; margin-left: auto; margin-right: auto; max-height: 563px; max-width: 948px; cursor: zoom-in;" width="947" height="563" alt="image" />

I extracted a sample of Active Area scanlines. As I explained before we
have a scanline containing the pixels of the graphics, and subsequently
an empty scanline (black) where no pixels are transmitted but the CM4
core has free access to the SRAM for rendering the graphics.

That said, let's analyze the active scanline, here's what happens.

The signal begins with the negative horizontal sync pulse, lasting
3.81us. For this purpose, I used a TCPWM counter which I set with the
"Device Manager".

<img src="/resized-image/__size/930x1494/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/2185.TCPWM.png" loading="lazy" style="display: block; height: 747px; margin-left: auto; margin-right: auto; max-height: 747px; max-width: 465px; cursor: zoom-in;" width="464" height="747" alt="image" />

Calculating the appropriate values is simple because the timer counts
the time at each clock cycle, which for peripherals is 75Mhz. So each
cycle lasts 13.33ns, and so to get the 3.81us pulse, I have to set the
Counter Compare 0 to 3810ns /13.33ns = 285.82 cycles, so 286. Along the
scanline, this is the HSync area. 

After that, there is the Back Porch that ends at the "Counter Compare
1", and at this point, the streaming of the pixels starts. So at the
compare 1 match, a second TCPWM is started. This one is the Pixel Clock,
which runs continuously until all pixels of the scanline are
transmitted.

<img src="/resized-image/__size/930x1532/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/7875.PixelCounter.png" loading="lazy" style="display: block; height: 766px; margin-left: auto; margin-right: auto; max-height: 766px; max-width: 465px; cursor: zoom-in;" width="464" height="533" alt="image" />

Here the Pixel Counter counts 7 cycles, and then it triggers a DMA
transfer, which copies one byte from the frame buffer to the GPIO port
connected to the RGB DAC. Here 7 cycles (0..6) correspond to 93.31ns,
which visually corresponds to the width of one pixel. Every DMA transfer
decrements the number of pixels to stream. This number is set in the DMA
configuration.

<img src="/resized-image/__size/942x1100/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/3755.DMAC.png" loading="lazy" style="display: block; height: 550px; margin-left: auto; margin-right: auto; max-height: 550px; max-width: 471px; cursor: zoom-in;" width="470" height="574" alt="image" />

As soon as the "Number of data elements to transfer" reaches 0 the Pixel
Counter is stopped. Why 257 since the horizontal resolution is 256? When
the GPIO port receives one byte from the DMA, sets the pins 0..7 to the
state of all bits of the byte.

<img src="/resized-image/__size/862x844/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/5722.GPIO.png" loading="lazy" style="display: block; height: 422px; margin-left: auto; margin-right: auto; max-height: 422px; max-width: 431px; cursor: zoom-in;" width="430" height="422" alt="image" />

The pins of the port remain in their state until a new transfer from the
DMA. That's why a pixel is 7 cycles wide, and that's why pixel 257 is
used to stop the RGB signal because it's always set to black. When the
first TCPWM reaches the end at cycle 2384 (2384 \* 13.33ns = 31.778us)
the scanline ends, the IRQ event fires, and the ISR is called. 

Within the ISR the scanlines are counted, and as we saw in the
schematic, the frame is composed.

After a scanline is shown the DMA is disabled, so the Pixel Counter
won't trigger a memory transfer and the SRAM is free for any operation
by the CM4. So the ISR sends a message to the CM4 core containing the
number of the next scanline to render, so for the next scanline pixels
data are ready to be streamed to GPIOs.

<span style="font-size:150%;">**2.2.2 - Audio Driver**</span>

Generating a digital audio signal is pretty simple and has several ways
to do it. We are fortunate that the PSoC6 has a Digital Analog Converter
(DAC) which can be configured to continuously convert a digital value
into an analog voltage level in real-time. To obtain a sound we just
need to vary this level over time, in order to create oscillations. This
signal applied to an amplifier and a speaker, is output as a sound.

To generate a sound with the DAC we need a buffer that contains a series
of data to be sent in sequence, and a counter/timer to send the data at
regular intervals to the DAC. The rate at which data is sent determines
the sample rate of the sound. To reproduce previously recorded sounds,
we must necessarily use the same sampling rate, but since the sound is
generated by the same application, i.e. the emulator, we have the
freedom to decide the frequency ourselves.

Since we need to synchronize audio to video, we can take advantage of
the video driver to send an audio sample to each scanline of the screen
to get as close as possible to a faithful reproduction of the audio
generated by the PSG.

I won't go into detail about the SN76489 PSG emulation, what is
important to know is that the code renders an audio sample for each
scanline in sequence at each call. Thanks to ModusToolbox, the basic
configuration of the DAC is very easy since we need only to send data to
it.

<img src="/resized-image/__size/1010x1562/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/1512.dac.png" loading="lazy" style="display: block; height: 781px; margin-left: auto; margin-right: auto; max-height: 781px; max-width: 505px; cursor: zoom-in;" width="504" height="684" alt="image" />

for sending the audio samples to the DAC, we just have to use the
appropriate function at the beginning of each scanline within the ISR of
the VGA driver.

<div
class="content-scrollable-wrapper content-scrollable-wrapper-scrolled"
style="max-width: 100%; overflow: auto; max-height: 668px;">

<div class="evolution-code-editor theme-clouds" style="">

<a href="#" class="fullscreen">Fullscreen</a>

<div class="ace_editor ace-clouds"
style="width: 100%; height: 34.2105px;">

<div class="ace_gutter">

<div class="ace_layer ace_gutter-layer ace_folding-enabled"
style="margin-top: 0px; height: 57.0175px; width: 39px;">

<div class="ace_gutter-cell" style="height: 11.4035px;">

1

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

2

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

3

</div>

</div>

<div class="ace_gutter-active-line" style="display: none;">

</div>

</div>

<div class="ace_scroller" style="left: 39px; right: 0px; bottom: 0px;">

<div class="ace_content"
style="margin-top: 0px; width: 577px; height: 57.0175px; margin-left: 0px;">

<div class="ace_layer ace_print-margin-layer">

<div class="ace_print-margin" style="left: 484px; visibility: hidden;">

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_text-layer" style="padding: 0px 4px;">

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">//</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_identifier">Cy_CTDAC_SetValue</span><span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">audio_dac_HW</span><span
class="ace_punctuation ace_operator">,</span> <span
class="ace_identifier">gAudioBufferFront</span><span
class="ace_paren ace_lparen">\[</span><span
class="ace_identifier">gVGAScanline</span><span
class="ace_paren ace_rparen">\])</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">//</span>

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_cursor-layer ace_hidden-cursors">

<div class="ace_cursor"
style="left: 4px; top: 0px; width: 6px; height: 11.4035px;">

</div>

</div>

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-v"
style="display: none; width: 22px; bottom: 0px;">

<div class="ace_scrollbar-inner"
style="width: 22px; height: 34.2105px;">

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-h"
style="display: none; height: 22px; left: 39px; right: 0px;">

<div class="ace_scrollbar-inner" style="height: 22px; width: 577px;">

</div>

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: hidden;">

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: visible;">

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font-style: inherit; font-variant: inherit; font-stretch: inherit; font-size: inherit; line-height: inherit; font-family: inherit; font-optical-sizing: inherit; font-kerning: inherit; font-feature-settings: inherit; font-variation-settings: inherit; overflow: visible;">

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

</div>

</div>

</div>

</div>

``` ui-code
//
    Cy_CTDAC_SetValue(audio_dac_HW, gAudioBufferFront[gVGAScanline]);
//
```

</div>

As already mentioned, the audio samples are read from a buffer, which
frame by frame is exchanged with a secondary buffer on which the
emulator has prepared new samples to be sent to the DAC. This technique
is called double buffering.

<div
class="content-scrollable-wrapper content-scrollable-wrapper-scrolled"
style="max-width: 100%; overflow: auto; max-height: 668px;">

<div class="evolution-code-editor theme-clouds" style="">

<a href="#" class="fullscreen">Fullscreen</a>

<div class="ace_editor ace-clouds"
style="width: 100%; height: 57.0175px;">

<div class="ace_gutter">

<div class="ace_layer ace_gutter-layer ace_folding-enabled"
style="margin-top: 0px; height: 79.8246px; width: 39px;">

<div class="ace_gutter-cell" style="height: 11.4035px;">

1

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

2

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

3

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

4

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

5

</div>

</div>

<div class="ace_gutter-active-line" style="display: none;">

</div>

</div>

<div class="ace_scroller" style="left: 39px; right: 0px; bottom: 0px;">

<div class="ace_content"
style="margin-top: 0px; width: 577px; height: 79.8246px; margin-left: 0px;">

<div class="ace_layer ace_print-margin-layer">

<div class="ace_print-margin" style="left: 484px; visibility: hidden;">

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_text-layer" style="padding: 0px 4px;">

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">// </span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_indent-guide"> </span> <span
class="ace_identifier">tmpPtr</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_identifier">gAudioBufferFront</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_indent-guide"> </span> <span
class="ace_identifier">gAudioBufferFront</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_identifier">gAudioBufferBack</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_indent-guide"> </span> <span
class="ace_identifier">gAudioBufferBack</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_identifier">tmpPtr</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">//</span>

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_cursor-layer ace_hidden-cursors">

<div class="ace_cursor"
style="left: 4px; top: 0px; width: 6px; height: 11.4035px;">

</div>

</div>

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-v"
style="display: none; width: 22px; bottom: 0px;">

<div class="ace_scrollbar-inner"
style="width: 22px; height: 57.0175px;">

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-h"
style="display: none; height: 22px; left: 39px; right: 0px;">

<div class="ace_scrollbar-inner" style="height: 22px; width: 577px;">

</div>

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: hidden;">

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: visible;">

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font-style: inherit; font-variant: inherit; font-stretch: inherit; font-size: inherit; line-height: inherit; font-family: inherit; font-optical-sizing: inherit; font-kerning: inherit; font-feature-settings: inherit; font-variation-settings: inherit; overflow: visible;">

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

</div>

</div>

</div>

</div>

``` ui-code
//    
      tmpPtr = gAudioBufferFront;
      gAudioBufferFront = gAudioBufferBack;
      gAudioBufferBack = tmpPtr;
//
```

</div>

<span style="font-size:150%;">**2.2.3 - Capsense Joypad Driver**</span>

The Capsense system of the Pioneer lets you configure and tune the
capacitive areas on board.

For this project, I wanted to use the example provided by ModusToolbox
as a starting point. This makes it more familiar to everyone. The
example configures the horizontal slider as a linear potentiometer,
while the two buttons as simple push-buttons. To turn it into a
capacitive joypad, I changed the main function to read the slider as if
it were a rotary encoder. 

This means that variations of increasing values on the slider correspond
to a movement to the right, while variations of decreasing values
correspond to movements to the left. Having said that it's just a matter
of coordinating the call to this refresh function at the time the screen
is not being refreshed, ie during the vertical blank area.  
What I think is important is to configure the controls well in the
Capsense Tuner.

<img src="/resized-image/__size/1558x820/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/6403.capsense_2D00_0.png" loading="lazy" style="display: block; height: 410px; margin-left: auto; margin-right: auto; max-height: 410px; max-width: 779px; cursor: zoom-in;" width="778" height="410" alt="image" />

To obtain "clean" and smooth movements it is necessary to instruct the
tuner by providing him with a profile on the background electromagnetic
noise and on the signal level when the areas are touched with the
fingers.

<img src="/resized-image/__size/1526x804/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/2273.capsense_2D00_noise.png" loading="lazy" style="display: block; height: 402px; margin-left: auto; margin-right: auto; max-height: 402px; max-width: 763px; cursor: zoom-in;" width="762" height="402" alt="image" />

<img src="/resized-image/__size/1520x802/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/3568.capsense_2D00_signal.png" loading="lazy" style="display: block; height: 401px; margin-left: auto; margin-right: auto; max-height: 401px; max-width: 760px; cursor: zoom-in;" width="759" height="376" alt="image" />

Once the profile has been created for all the capacitive areas, it is
possible to test the controls, either through graphs or by viewing the
levels. I am amazed by the Capsense system because it's unbelievable the
amount of time and work that saves you.

<img src="/resized-image/__size/1518x802/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/8358.capsense_2D00_signal_2D00_2.png" loading="lazy" style="display: block; height: 401px; margin-left: auto; margin-right: auto; max-height: 401px; max-width: 759px; cursor: zoom-in;" width="758" height="368" alt="image" />

<img src="/resized-image/__size/1516x798/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/4130.capsense_2D00_test_2D00_slider.png" loading="lazy" style="display: block; height: 399px; margin-left: auto; margin-right: auto; max-height: 399px; max-width: 758px; cursor: zoom-in;" width="757" height="399" alt="image" />

I'll leave the details in the source code for further reading.

<span style="font-size:150%;">**2.3 - Software side on CM4**</span>

Since the CM4 core is faster, it takes care of running the entire
emulator but lets the CM0+ core handle the synchronization. Explaining
and detailing the hardware of the SEGA SG-1000 console would be long and
cumbersome in this context, so if it is a topic of interest, I will
continue the project later. The important point is how I made the two
cores communicate.

In section 2.0 - Architecture, I showed that both cores have read and
write access to specific memory locations. These locations function as
memory-mapped registers. As I explained in this blog, I preferred to use
this system because I found it to be faster than the IPC.  
Since the CM0+ core needs to request the next scanline to be rendered
quickly while a black line is displayed, reaction times are crucial.

<span style="font-size:150%;">**2.3.1 - Memory mapped registers**</span>

This system is very simple and relies on the use of SRAM which is shared
by the cores. In practice, one core writes a value to one of these
registers while the other core waits for that given register to assume a
non-zero value. The calling core, after having written the value,
eventually waits for it to return to zero, a sign that the second core
has read it and therefore reset it.

This method is simple but care must be taken to handle pointers well.
Better to create a common file for both cores, with the definition of an
address map in the SRAM.

<div
class="content-scrollable-wrapper content-scrollable-wrapper-scrolled"
style="max-width: 100%; overflow: auto; max-height: 668px;">

<div class="evolution-code-editor theme-clouds" style="">

<a href="#" class="fullscreen">Fullscreen</a>

<div class="ace_editor ace-clouds"
style="width: 100%; height: 228.07px;">

<div class="ace_gutter">

<div class="ace_layer ace_gutter-layer ace_folding-enabled"
style="margin-top: 0px; height: 250.877px; width: 45px;">

<div class="ace_gutter-cell" style="height: 11.4035px;">

1

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

2

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

3

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

4

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

5

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

6

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

7

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

8

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

9

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

10

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

11

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

12

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

13

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

14

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

15

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

16

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

17

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

18

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

19

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

20

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

21

</div>

</div>

<div class="ace_gutter-active-line" style="display: none;">

</div>

</div>

<div class="ace_scroller" style="left: 45px; right: 17px; bottom: 0px;">

<div class="ace_content"
style="margin-top: 0px; width: 554px; height: 250.877px; margin-left: 0px;">

<div class="ace_layer ace_print-margin-layer">

<div class="ace_print-margin" style="left: 484px; visibility: hidden;">

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_text-layer" style="padding: 0px 4px;">

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">// Within the source file \*.c</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gCommandAddr</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">SHARED_MEMORY</span><span
class="ace_paren ace_rparen">)</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gCmdAddr</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">SHARED_MEMORY</span> <span
class="ace_keyword ace_operator">+</span> <span
class="ace_constant ace_numeric">4</span><span
class="ace_paren ace_rparen">)</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gVBlankAddr</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">SHARED_MEMORY</span> <span
class="ace_keyword ace_operator">+</span> <span
class="ace_constant ace_numeric">8</span><span
class="ace_paren ace_rparen">)</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gVideoBufferAddr</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">SHARED_MEMORY</span> <span
class="ace_keyword ace_operator">+</span> <span
class="ace_constant ace_numeric">12</span><span
class="ace_paren ace_rparen">)</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gAudioBufferAddr</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">SHARED_MEMORY</span> <span
class="ace_keyword ace_operator">+</span> <span
class="ace_constant ace_numeric">16</span><span
class="ace_paren ace_rparen">)</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gIOAddr</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">SHARED_MEMORY</span> <span
class="ace_keyword ace_operator">+</span> <span
class="ace_constant ace_numeric">20</span><span
class="ace_paren ace_rparen">)</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_punctuation ace_operator">...</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">// Within the header file \*.h</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">extern</span> <span
class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gCommandAddr</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">extern</span> <span
class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gCmdAddr</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">extern</span> <span
class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gVBlankAddr</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">extern</span> <span
class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gVideoBufferAddr</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">extern</span> <span
class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gAudioBufferAddr</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_storage ace_modifier">extern</span> <span
class="ace_storage ace_modifier">volatile</span> <span
class="ace_identifier">uint32_t</span> <span
class="ace_identifier">gIOAddr</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">// macros</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_keyword">#define</span><span
class="ace_constant ace_other"> \_SHARED_gCommand
\*((uint8_t\*)(\*((uint32_t\*)gCmdAddr)))</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_keyword">#define</span><span
class="ace_constant ace_other"> \_SHARED_gVBlank
\*((uint8_t\*)(\*((uint32_t\*)gVBlankAddr)))</span>

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_cursor-layer ace_hidden-cursors">

<div class="ace_cursor"
style="left: 4px; top: 0px; width: 6px; height: 11.4035px;">

</div>

</div>

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-v"
style="width: 22px; bottom: 0px;">

<div class="ace_scrollbar-inner"
style="width: 22px; height: 376.316px;">

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-h"
style="display: none; height: 22px; left: 45px; right: 17px;">

<div class="ace_scrollbar-inner" style="height: 22px; width: 554px;">

</div>

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: hidden;">

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: visible;">

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font-style: inherit; font-variant: inherit; font-stretch: inherit; font-size: inherit; line-height: inherit; font-family: inherit; font-optical-sizing: inherit; font-kerning: inherit; font-feature-settings: inherit; font-variation-settings: inherit; overflow: visible;">

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

</div>

</div>

</div>

</div>

``` ui-code
// Within the source file *.c
volatile uint32_t gCommandAddr      = (SHARED_MEMORY);
volatile uint32_t gCmdAddr          = (SHARED_MEMORY + 4);
volatile uint32_t gVBlankAddr       = (SHARED_MEMORY + 8);
volatile uint32_t gVideoBufferAddr  = (SHARED_MEMORY + 12);
volatile uint32_t gAudioBufferAddr  = (SHARED_MEMORY + 16);
volatile uint32_t gIOAddr           = (SHARED_MEMORY + 20);

...

// Within the header file *.h
extern volatile uint32_t gCommandAddr;
extern volatile uint32_t gCmdAddr;
extern volatile uint32_t gVBlankAddr;
extern volatile uint32_t gVideoBufferAddr;
extern volatile uint32_t gAudioBufferAddr;
extern volatile uint32_t gIOAddr;

// macros
#define _SHARED_gCommand  *((uint8_t*)(*((uint32_t*)gCmdAddr)))
#define _SHARED_gVBlank   *((uint8_t*)(*((uint32_t*)gVBlankAddr)))
#define _SHARED_gIOPort   *((uint8_t*)(*((uint32_t*)gIOAddr)))

...

// access methods
// writing:
_SHARED_gCommand = n;

// reading:
value = _SHARED_gCommand;

#define _SHARED_gSemaPort *((uint8_t*)(*((uint32_t*)gSemaAddr)))
```

</div>

Later, where necessary, it will be easy to write to the registers and
create a waiting cycle for them. In the following example, the core CM4
is waiting to receive a value from the core CM0+.

<div
class="content-scrollable-wrapper content-scrollable-wrapper-scrolled"
style="max-width: 100%; overflow: auto; max-height: 668px;">

<div class="evolution-code-editor theme-clouds" style="">

<a href="#" class="fullscreen">Fullscreen</a>

<div class="ace_editor ace-clouds"
style="width: 100%; height: 68.4211px;">

<div class="ace_gutter">

<div class="ace_layer ace_gutter-layer ace_folding-enabled"
style="margin-top: 0px; height: 91.2281px; width: 39px;">

<div class="ace_gutter-cell" style="height: 11.4035px;">

1

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

2

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

3<span class="ace_fold-widget ace_start ace_open"
style="height: 11.4035px;"></span>

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

4

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

5

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

6

</div>

</div>

<div class="ace_gutter-active-line" style="display: none;">

</div>

</div>

<div class="ace_scroller" style="left: 39px; right: 0px; bottom: 0px;">

<div class="ace_content"
style="margin-top: 0px; width: 577px; height: 91.2281px; margin-left: 0px;">

<div class="ace_layer ace_print-margin-layer">

<div class="ace_print-margin" style="left: 484px; visibility: hidden;">

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_text-layer" style="padding: 0px 4px;">

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">// cm0+</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_keyword ace_control">for</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_punctuation ace_operator">;;</span><span
class="ace_paren ace_rparen">)</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_paren ace_lparen">{</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_indent-guide"> </span> <span
class="ace_identifier">\_SHARED_gCommand</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_constant ace_numeric">1</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_indent-guide"> </span> <span
class="ace_keyword ace_control">while</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">\_SHARED_gCommand</span><span
class="ace_paren ace_rparen">)</span> <span
class="ace_paren ace_lparen">{</span><span
class="ace_paren ace_rparen">}</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_paren ace_rparen">}</span>

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_cursor-layer ace_hidden-cursors">

<div class="ace_cursor"
style="left: 4px; top: 0px; width: 6px; height: 11.4035px;">

</div>

</div>

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-v"
style="display: none; width: 22px; bottom: 0px;">

<div class="ace_scrollbar-inner"
style="width: 22px; height: 68.4211px;">

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-h"
style="display: none; height: 22px; left: 39px; right: 0px;">

<div class="ace_scrollbar-inner" style="height: 22px; width: 577px;">

</div>

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: hidden;">

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: visible;">

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font-style: inherit; font-variant: inherit; font-stretch: inherit; font-size: inherit; line-height: inherit; font-family: inherit; font-optical-sizing: inherit; font-kerning: inherit; font-feature-settings: inherit; font-variation-settings: inherit; overflow: visible;">

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

</div>

</div>

</div>

</div>

``` ui-code
// cm0+
    for (;;)
    {
      _SHARED_gCommand = 1;
      while (_SHARED_gCommand) {};
    }
```

</div>

<div
class="content-scrollable-wrapper content-scrollable-wrapper-scrolled"
style="max-width: 100%; overflow: auto; max-height: 668px;">

<div class="evolution-code-editor theme-clouds" style="">

<a href="#" class="fullscreen">Fullscreen</a>

<div class="ace_editor ace-clouds"
style="width: 100%; height: 68.4211px;">

<div class="ace_gutter">

<div class="ace_layer ace_gutter-layer ace_folding-enabled"
style="margin-top: 0px; height: 91.2281px; width: 39px;">

<div class="ace_gutter-cell" style="height: 11.4035px;">

1

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

2

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

3<span class="ace_fold-widget ace_start ace_open"
style="height: 11.4035px;"></span>

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

4

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

5

</div>

<div class="ace_gutter-cell" style="height: 11.4035px;">

6

</div>

</div>

<div class="ace_gutter-active-line" style="display: none;">

</div>

</div>

<div class="ace_scroller" style="left: 39px; right: 0px; bottom: 0px;">

<div class="ace_content"
style="margin-top: 0px; width: 577px; height: 91.2281px; margin-left: 0px;">

<div class="ace_layer ace_print-margin-layer">

<div class="ace_print-margin" style="left: 484px; visibility: hidden;">

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_text-layer" style="padding: 0px 4px;">

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_comment">// cm4</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_keyword ace_control">for</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_punctuation ace_operator">;;</span><span
class="ace_paren ace_rparen">)</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_paren ace_lparen">{</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_indent-guide"> </span> <span
class="ace_keyword ace_control">while</span> <span
class="ace_paren ace_lparen">(</span><span
class="ace_identifier">\_SHARED_gCommand</span> <span
class="ace_keyword ace_operator">==</span> <span
class="ace_constant ace_numeric">0</span><span
class="ace_paren ace_rparen">)</span> <span
class="ace_paren ace_lparen">{</span><span
class="ace_paren ace_rparen">}</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_indent-guide"> </span> <span
class="ace_identifier">\_SHARED_gCommand</span> <span
class="ace_keyword ace_operator">=</span> <span
class="ace_constant ace_numeric">0</span><span
class="ace_punctuation ace_operator">;</span>

</div>

<div class="ace_line" style="height:11.403509140014648px">

<span class="ace_paren ace_rparen">}</span>

</div>

</div>

<div class="ace_layer ace_marker-layer">

</div>

<div class="ace_layer ace_cursor-layer ace_hidden-cursors">

<div class="ace_cursor"
style="left: 4px; top: 0px; width: 6px; height: 11.4035px;">

</div>

</div>

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-v"
style="display: none; width: 22px; bottom: 0px;">

<div class="ace_scrollbar-inner"
style="width: 22px; height: 68.4211px;">

</div>

</div>

<div class="ace_scrollbar ace_scrollbar-h"
style="display: none; height: 22px; left: 39px; right: 0px;">

<div class="ace_scrollbar-inner" style="height: 22px; width: 577px;">

</div>

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: hidden;">

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font: inherit; overflow: visible;">

</div>

<div
style="height: auto; width: auto; top: 0px; left: 0px; visibility: hidden; position: absolute; white-space: pre; font-style: inherit; font-variant: inherit; font-stretch: inherit; font-size: inherit; line-height: inherit; font-family: inherit; font-optical-sizing: inherit; font-kerning: inherit; font-feature-settings: inherit; font-variation-settings: inherit; overflow: visible;">

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

</div>

</div>

</div>

</div>

``` ui-code
// cm4
    for (;;)
    {
      while (_SHARED_gCommand == 0) {};
      _SHARED_gCommand = 0;
    }
```

</div>

<span style="font-size:150%;">**2.3.2 - Emulator Architecture**</span>

A software computer emulator has to emulate all the components in a real
computer using software programs over another computer or hardware.
Basically, it's based on a virtual machine (VM) like Java and others. A
virtual machine is any kind of computer that does not exist as real
hardware but it is implemented by software. All hardware components that
are in the hardware to emulate, actually are translated into code that
will mimic as accurately as possible the behavior of the component.

In section 2.0 - Architecture, is shown the main schematic of the SEGA
SG-1000 hardware. In my emulator, the various active components perform
certain functions that can be called cyclically such as rendering
graphics or video. In theory, to simulate hardware in real-time, all the
components should execute a piece of program each in sequence, thus
exploiting a memory-mapped register, at each scanline the CM0+ core
calls the code parts in the CM4 core which executes program parts of the
game.

Now, the problem that arises is respecting the times of the Video
Driver, in fact, a video scanline lasts 31.778us, therefore the
emulation corresponding to the time that the console takes to calculate
everything necessary to draw a scanline, must last that time. This is
very difficult, and a 150MHz CM4 core frequency is a bit low to do
everything.

What I thought of doing is dividing the tasks into two different
moments.

I will try to explain as best as possible and briefly how the SG-1000
works.

As soon as you turn on the console, the Z80 CPU starts reading the code
from the ROM contained in the cartridge. Running game code will ask the
Z80 to read and write data to RAM, prepare graphics by accessing VDP
registers, or change frequency to PSG audio oscillators.

The VDP and PSG also have continuous output which is affected by
internal registers which are modified by the Z80. Usually to obtain some
stability the Z80 accesses these components when the VDP is not drawing
the video frame. Then, to reduce the number of operations that the CM4
core has to perform in the time of a scanline, I can render only the
graphics during the Active Area of the VGA signal, and subsequently
during the vertical blank area, run the Z80 and render the samples
audio.

<span style="font-size:150%;">**3.0 - Conclusion**</span>

Summing up, this project allowed me to get to know the PSoC62, and at
the same time share a learning path with all of you, developing an
application that could push this MCU to the maximum.

An emulator requires a lot of resources, and so I set myself this
challenge, but what are the limitations I've encountered?  
First the 128Kb memory for a project that uses graphics and sound is
limited, especially if you need a video frame buffer to render to.
Suffice it to say that if we want to implement an RGB video driver we
have to spend 76800 bytes for a frame buffer with a resolution of
320x240. This emulator has been heavily modified to run "comfortably"
with this amount of SRAM.

However this emulator could support all titles released for SEGA
SG-1000, but obviously, in the short time available it was tested only
for the proposed game "Space Invaders". On the other hand, a heavy
limitation for the choice of the game is given by the presence of only
one axis for the capacitive joypad.

As far as speed is concerned, you have to pay close attention to how to
configure the clocks, bearing in mind that both cores share the same
clock source and the frequency of the two cores must be balanced with
dividers, therefore the maximum obtainable is 150Mhz on CM4 and 75MHz on
CM0+. If, on the other hand, you want to reach 100MHz on CM0+, the CM4
core also drops to 100MHz.

Another aspect to consider is the use of memory which is shared, and for
correct use and based on the application to be developed, it may require
the configuration of the protection units, as well as the
synchronization of accesses by the two cores.

Otherwise, the tools offered by ModusToolbox are extremely useful and
easy to use. I am very happy with this experience, and I hope I have
left something for everyone by turning the Pioneer Kit into a mini-game
console.

<span style="font-size:150%;">**3.1 - The project**</span>

This project is available on GitHub so that you all can contribute to it
later on if you like it.

[Digimorf GitHub Element14&Infineon "At The Core Design
Challenge"](https://github.com/Digimorf/Element14-and-Infineon--At-The-Core-Design-Challenge "Digimorf GitHub Element14&Infineon "At The Core Design Challenge"")

Anyway, I am attaching the compiled \*.hex file below which can be
loaded into the FLASH of the board for ready use.

[0820.ATCDC_PSoC62_SG-1000.zip](https://community.element14.com/cfs-file/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/0820.ATCDC_5F00_PSoC62_5F00_SG_2D00_1000.zip)

<div style="clear:both;">

</div>

</div>

</div>

</div>

<div class="content-fragment-footer">

</div>

</div>

<div id="fragment-7298"
class="content-fragment threaded-comments no-wrapper with-spacing responsive-1"
reflow="_p_content,_p_singlecolumn,1,1,12">

<div class="content-fragment-content">

<div class="comment-form">

<div id="fragment-7298_statisticsWrapper" class="comment-statistics">

-   <span class="value prepend-icon comment"> 9 comments </span>
-   <span class="value prepend-icon user"> 2 members are here </span>

</div>

<div class="author" authorprofileurl="/members/digimorf">

<div class="avatar">

<a href="/members/digimorf" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/24/55/56/4UJ9DW4IF7NM.jpg-44x44x2.jpg?_=skoPvJtq8/g3Z5q0/xSYxw==" data-border="0px" alt="Digimorf" /></a>

</div>

<div class="meta">

<span class="user-name"> <span class="ui-userpresence ui-tip present"
userid="245556" contentid="d332699e-bc20-475a-9cae-2d7a3e2c034c"
contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="true"
presentlabel="Online" notpresentlabel="Offline"
tip="Online">Online</span>
<a href="/members/digimorf" class="internal-link view-user-profile">Digimorf</a>
</span>

</div>

</div>

-   <span class="field-item-input"> </span>
    <div id="mceu_7"
    class="mce-tinymce mce-container mce-panel mce-editor-submittable mce-editor-desktop mce-editor-blur"
    hidefocus="1" tabindex="-1" role="application"
    style="visibility: hidden; border-width: 1px; height: auto; overflow: visible;">

    <div id="mceu_7-body" class="mce-container-body mce-stack-layout">

    <div id="mceu_8"
    class="mce-container mce-menubar mce-toolbar mce-stack-layout-item mce-first"
    role="menubar" style="border-width: 0px 0px 1px; display: none;">

    <div id="mceu_8-body" class="mce-container-body mce-flow-layout">

    <div id="mceu_9"
    class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-first mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_9" role="menuitem"
    aria-haspopup="true">

    <span class="mce-txt">Edit</span>

    </div>

    <div id="mceu_10"
    class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_10" role="menuitem"
    aria-haspopup="true">

    <span class="mce-txt">Insert</span>

    </div>

    <div id="mceu_11"
    class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_11" role="menuitem"
    aria-haspopup="true">

    <span class="mce-txt">Format</span>

    </div>

    <div id="mceu_12"
    class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-last mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_12" role="menuitem"
    aria-haspopup="true">

    <span class="mce-txt">Tools</span>

    </div>

    </div>

    </div>

    <div id="mceu_16"
    class="mce-edit-area mce-container mce-panel mce-stack-layout-item mce-last"
    hidefocus="1" tabindex="-1" role="group"
    style="border-width: 0px 0px 1px; position: relative;">

    <div class="mce-placeholder">

    Leave a comment...

    </div>

    <div style="position: absolute; top: 0px; left: 0px;">

    <div class="mceDragDropShade"
    style="position: absolute; display: none; left: 0px; top: 0px;">

    </div>

    <div class="mceDragDropMessage"
    style="position: absolute; display: none; left: 0px; top: 0px;">

    </div>

    </div>

    </div>

    <div id="mceu_13"
    class="mce-toolbar-grp mce-container mce-panel mce-stack-layout-item"
    hidefocus="1" tabindex="-1" role="group">

    <div id="mceu_13-body" class="mce-container-body mce-stack-layout">

    <div id="mceu_14"
    class="mce-container mce-toolbar mce-stack-layout-item mce-first mce-last"
    role="toolbar">

    <div id="mceu_14-body" class="mce-container-body mce-flow-layout">

    <div id="mceu_15"
    class="mce-container mce-flow-layout-item mce-first mce-last mce-btn-group"
    role="group" style="width: 100%;">

    <div id="mceu_15-body" style="white-space: normal;">

    <div id="mceu_0"
    class="mce-widget mce-btn mce-menubtn mce-first mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_0" role="button"
    aria-haspopup="true">

    <span class="mce-txt">Edit</span>

    </div>

    <div id="mceu_1"
    class="mce-widget mce-btn mce-menubtn mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_1" role="button"
    aria-haspopup="true">

    <span class="mce-txt">Insert</span>

    </div>

    <div id="mceu_2"
    class="mce-widget mce-btn mce-menubtn mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_2" role="button"
    aria-haspopup="true">

    <span class="mce-txt">Format</span>

    </div>

    <div id="mceu_3"
    class="mce-widget mce-btn mce-menubtn mce-btn-has-text"
    tabindex="-1" aria-labelledby="mceu_3" role="button"
    aria-haspopup="true">

    <span class="mce-txt">Tools</span>

    </div>

    <div id="mceu_4" class="mce-widget mce-btn" tabindex="-1"
    aria-labelledby="mceu_4" role="button" aria-label="Bullet list">

    </div>

    <div id="mceu_5" class="mce-widget mce-btn" tabindex="-1"
    aria-labelledby="mceu_5" role="button" aria-label="Numbered list">

    </div>

    <div id="mceu_6" class="mce-widget mce-submitpanel mce-last"
    role="presentation" aria-haspopup="false">

    <div id="mceu_6_send"
    class="mce-widget mce-btn mce-primary mce-btn-has-text mce-submitpanel-button"
    tabindex="0" role="button">

    <span class="mce-txt">Comment</span>

    </div>

    </div>

    </div>

    </div>

    </div>

    </div>

    </div>

    </div>

    </div>

    <div
    style="position: absolute; z-index: -1; height: 1px; width: 1px; overflow: hidden;">

    </div>

    </div>

    <span class="field-item-validation" style="display: none;"></span>
    <span class="processing"><span class="ui-loading" data-width="45"
    data-height="15"
    style="display: block; width: 45px; height: 15px; margin: 0px auto; padding: 0px; text-align: center; box-sizing: border-box;"></span></span>
    <div class="spinner">

    <div class="bounce1"
    style="width:15px;height:15px;background-color:rgb(51, 51, 51);">

    </div>

    <div class="bounce2"
    style="width:15px;height:15px;background-color:rgb(51, 51, 51);">

    </div>

    <div class="bounce3"
    style="width:15px;height:15px;background-color:rgb(51, 51, 51);">

    </div>

    </div>

</div>

<div id="fragment-7298_bestRepliesWrapper" class="best-replies-wrapper">

<div class="threaded-summary">

## Top Comments

-   <div class="author" authorprofileurl="/members/navadeepganeshu">

    <div class="avatar">

    <a href="/members/navadeepganeshu" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/32/31/16/4UFGPOCIFHEZ.jpg-44x44x2.jpg?_=BVgG1/Py3X1KerCoyTfVzg==" data-border="0px" alt="navadeepganeshu" /></a>

    </div>

    <div class="meta">

    <span class="user-name"> <span class="ui-userpresence ui-tip"
    userid="323116" contentid="4adfb4f7-24a6-4c88-b740-ad188d1b5574"
    contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="false"
    presentlabel="Online" notpresentlabel="Offline"
    tip="Offline">Offline</span>
    <a href="/members/navadeepganeshu" class="internal-link view-user-profile">navadeepganeshu</a>
    </span>
    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=51462d43-9cb6-40d8-b21d-417d06b520ae" class="internal-link view-post navigable"><span class="ui-agodate ui-tip" data-tip="5 May 2023 3:32 PM" data-dateutc="2023-05-05T15:32:10.290Z">2 months ago</span></a>
    <span class="votes">+1</span>

    </div>

    </div>

    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=51462d43-9cb6-40d8-b21d-417d06b520ae" class="internal-link view-full-post navigable ui-tip"></a>
    <div class="content user-defined-markup">

    Great project. I liked the way you depicted the while system
    architecture. Super pleasing and technical at the same time. I am
    too bored with flowcharts and numbered algorithms...

    </div>

-   <div class="author" authorprofileurl="/members/dougw">

    <div class="avatar">

    <a href="/members/dougw" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/01/66/37/4UEYEAB88T8D.png-44x44x2.png?_=s+tKfYvQfvPoTdTDvv+m6g==" data-border="0px" alt="dougw" /></a>

    </div>

    <div class="meta">

    <span class="user-name"> <span class="ui-userpresence ui-tip"
    userid="16637" contentid="193ac243-4c81-4a24-b466-35a2210baec1"
    contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="false"
    presentlabel="Online" notpresentlabel="Offline"
    tip="Offline">Offline</span>
    <a href="/members/dougw" class="internal-link view-user-profile">dougw</a>
    </span>
    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=91c70a35-f8bf-4d58-9791-f3faeb6d382c" class="internal-link view-post navigable"><span class="ui-agodate ui-tip" data-tip="6 May 2023 3:12 AM" data-dateutc="2023-05-06T03:12:11.627Z">2 months ago</span></a>
    <span class="votes">+1</span>

    </div>

    </div>

    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=91c70a35-f8bf-4d58-9791-f3faeb6d382c" class="internal-link view-full-post navigable ui-tip"></a>
    <div class="content user-defined-markup">

    Spectacular project! Well done.

    </div>

-   <div class="author" authorprofileurl="/members/dab">

    <div class="avatar">

    <a href="/members/dab" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/10/43/35/4UEYJN8GSL6S.png-44x44x2.png?_=DYxI/KXozB5/dcOqH5/Pew==" data-border="0px" alt="DAB" /></a>

    </div>

    <div class="meta">

    <span class="user-name"> <span class="ui-userpresence ui-tip"
    userid="104335" contentid="4966e6ad-97f6-4dac-9ea8-919abb220e1d"
    contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="false"
    presentlabel="Online" notpresentlabel="Offline"
    tip="Offline">Offline</span>
    <a href="/members/dab" class="internal-link view-user-profile">DAB</a>
    </span>
    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=ddceb963-2693-4753-ad0d-d4a34ce49e6b" class="internal-link view-post navigable"><span class="ui-agodate ui-tip" data-tip="6 May 2023 8:27 PM" data-dateutc="2023-05-06T20:27:59.953Z">2 months ago</span></a>
    <span class="votes">+1</span>

    </div>

    </div>

    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=ddceb963-2693-4753-ad0d-d4a34ce49e6b" class="internal-link view-full-post navigable ui-tip"></a>
    <div class="content user-defined-markup">

    Very good project, well explained.

    </div>

</div>

</div>

<div class="threaded-wrapper-outer">

<div class="typing-status-wrapper root">

</div>

<div id="fragment-7298_preRenderedWrapperId" class="threaded-wrapper"
style="display: block;">

<div class="reply-header">

<div id="fragment-7298_filter" class="threaded-filter">

-   <a href="?CommentSortBy=CreatedDate&amp;CommentSortOrder=Ascending" class="navigable">Oldest</a>
-   <a href="?CommentSortBy=Votes&amp;CommentSortOrder=Descending" class="navigable">Best</a>
-   <a href="?CommentSortBy=CreatedDate&amp;CommentSortOrder=Descending" class="navigable">Newest</a>

</div>

</div>

-   <div class="rendered-content">

    <div class="author" authorprofileurl="/members/navadeepganeshu">

    <div class="avatar">

    <a href="/members/navadeepganeshu" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/32/31/16/4UFGPOCIFHEZ.jpg-44x44x2.jpg?_=BVgG1/Py3X1KerCoyTfVzg==" data-border="0px" alt="navadeepganeshu" /></a>

    </div>

    <div class="meta">

    <span class="user-name"> <span class="ui-userpresence ui-tip"
    userid="323116" contentid="4adfb4f7-24a6-4c88-b740-ad188d1b5574"
    contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="false"
    presentlabel="Online" notpresentlabel="Offline"
    tip="Offline">Offline</span>
    <a href="/members/navadeepganeshu" class="internal-link view-user-profile">navadeepganeshu</a>
    </span>
    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=51462d43-9cb6-40d8-b21d-417d06b520ae" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="5 May 2023 3:32 PM" data-dateutc="2023-05-05T15:32:10.290Z">2 months ago</span></a>

    </div>

    </div>

    <div
    class="content full threaded-reply-content user-defined-markup">

    <div class="content">

    Great project. I liked the way you depicted the while
    system architecture. Super pleasing and technical at the same time.

    I am too bored with flowcharts and numbered algorithms... <span
    class="emoticon ui-tip"
    title="Expressionless"><img src="https://community.element14.com/cfs-file/__key/system/emoji/1f611.svg" style="max-height: 32px;max-width: 32px;" alt="Expressionless" /></span>

    <div style="clear:both;">

    </div>

    </div>

    </div>

    <div class="edit-form">

    </div>

    <div class="actions meta edit">

    <div class="navigation-list" maxlinks="1" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null">

    </div>

    </div>

    <div class="actions meta default">

    <div class="navigation-list" maxlinks="3" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null"
    finalized="true" style="overflow: hidden; height: 21.2993px;">

    <div class="container"
    style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

    -   <span class="votes" style="">
        <a href="#" class="ui-tip vote up selected">Vote Up</a> <span
        class="vote current ui-tip" tip="View Voters"
        replyid="51462d43-9cb6-40d8-b21d-417d06b520ae">+1</span>
        <a href="#" class="ui-tip vote down">Vote Down</a> </span>
    -   <span class="ui-presentcontent ui-like ui-tip"
        presentcontents="51462d439cb640d8b21d417d06b520ae|9262536b49a34494802f04dff10424ed|"
        tipcontent="initialmessage"
        contentid="51462d43-9cb6-40d8-b21d-417d06b520ae"
        contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
        initialcount="4" initialstate="true" data-readonly="false"
        initialmessage="You and &lt;span class=&quot;who-likes&quot;&gt;3 others&lt;/span&gt; like this"
        format="{toggle} {count}"
        configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
        style=""><span
        class="like-toggle"><a href="#" class="internal-link like-on">Unlike</a></span>
        <span class="like-count who-likes" count="4">4</span></span>
    -   <a href="#" class="internal-link new-reply">Reply</a>
    -   
    -   <a href="#" class="more">More</a>

    </div>

    </div>

    </div>

    <div class="typing-status-wrapper">

    </div>

    <div class="new-replies-wrapper">

    </div>

    </div>

    <div class="newreply">

    </div>

    -   <div class="rendered-content">

        <div class="author" authorprofileurl="/members/digimorf">

        <div class="avatar">

        <a href="/members/digimorf" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/24/55/56/4UJ9DW4IF7NM.jpg-44x44x2.jpg?_=skoPvJtq8/g3Z5q0/xSYxw==" data-border="0px" alt="Digimorf" /></a>

        </div>

        <div class="meta">

        <span class="user-name"> <span
        class="ui-userpresence ui-tip present" userid="245556"
        contentid="d332699e-bc20-475a-9cae-2d7a3e2c034c"
        contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05"
        present="true" presentlabel="Online" notpresentlabel="Offline"
        tip="Online">Online</span>
        <a href="/members/digimorf" class="internal-link view-user-profile">Digimorf</a>
        </span>
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=c82aa23d-64fd-4d1b-ba7c-7e2a1e709b95" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="6 May 2023 10:27 AM" data-dateutc="2023-05-06T10:27:41.160Z">2 months ago</span></a>
        <span class="parent"> in reply to
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=51462d43-9cb6-40d8-b21d-417d06b520ae" class="internal-link view-post navigable ui-tip">navadeepganeshu</a>
        </span>

        </div>

        </div>

        <div
        class="content full threaded-reply-content user-defined-markup">

        <div class="content">

        Thanks, I am glad you like it! 

        <div style="clear:both;">

        </div>

        </div>

        </div>

        <div class="edit-form">

        </div>

        <div class="actions meta edit">

        <div class="navigation-list" maxlinks="1" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null">

        </div>

        </div>

        <div class="actions meta default">

        <div class="navigation-list" maxlinks="3" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null"
        finalized="true" style="overflow: hidden; height: 21.2993px;">

        <div class="container"
        style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

        -   <span class="votes" style="">
            <a href="#" class="ui-tip vote up">Vote Up</a> <span
            class="vote current ui-tip" tip="View Voters"
            replyid="c82aa23d-64fd-4d1b-ba7c-7e2a1e709b95">0</span>
            <a href="#" class="ui-tip vote down">Vote Down</a> </span>
        -   <span class="ui-presentcontent ui-like ui-tip"
            presentcontents="c82aa23d64fd4d1bba7c7e2a1e709b95|9262536b49a34494802f04dff10424ed|"
            tipcontent="initialmessage"
            contentid="c82aa23d-64fd-4d1b-ba7c-7e2a1e709b95"
            contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
            initialcount="4" initialstate="false" data-readonly="false"
            initialmessage="&lt;a href=&quot;/members/ralphjy&quot; class=&quot;internal-link view-user-profile&quot;&gt;ralphjy&lt;/a&gt; and &lt;span class=&quot;who-likes&quot;&gt;3 others&lt;/span&gt; like this"
            format="{toggle} {count}"
            configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
            style=""><span
            class="like-toggle"><a href="#" class="internal-link like-off">Like</a></span>
            <span class="like-count who-likes" count="4">4</span></span>
        -   <a href="#" class="internal-link new-reply">Reply</a>
        -   
        -   
        -   <a href="#" class="more">More</a>

        </div>

        </div>

        </div>

        <div class="typing-status-wrapper">

        </div>

        <div class="new-replies-wrapper">

        </div>

        </div>

        <div class="newreply">

        </div>

-   <div class="rendered-content">

    <div class="author" authorprofileurl="/members/dougw">

    <div class="avatar">

    <a href="/members/dougw" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/01/66/37/4UEYEAB88T8D.png-44x44x2.png?_=s+tKfYvQfvPoTdTDvv+m6g==" data-border="0px" alt="dougw" /></a>

    </div>

    <div class="meta">

    <span class="user-name"> <span class="ui-userpresence ui-tip"
    userid="16637" contentid="193ac243-4c81-4a24-b466-35a2210baec1"
    contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="false"
    presentlabel="Online" notpresentlabel="Offline"
    tip="Offline">Offline</span>
    <a href="/members/dougw" class="internal-link view-user-profile">dougw</a>
    </span>
    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=91c70a35-f8bf-4d58-9791-f3faeb6d382c" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="6 May 2023 3:12 AM" data-dateutc="2023-05-06T03:12:11.627Z">2 months ago</span></a>

    </div>

    </div>

    <div
    class="content full threaded-reply-content user-defined-markup">

    <div class="content">

    Spectacular project! Well done. <span class="emoticon ui-tip"
    title="Ok hand"><img src="https://community.element14.com/cfs-file/__key/system/emoji/1f44c.svg" style="max-height: 32px;max-width: 32px;" alt="Ok hand" /></span>

    <div style="clear:both;">

    </div>

    </div>

    </div>

    <div class="edit-form">

    </div>

    <div class="actions meta edit">

    <div class="navigation-list" maxlinks="1" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null">

    </div>

    </div>

    <div class="actions meta default">

    <div class="navigation-list" maxlinks="3" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null"
    finalized="true" style="overflow: hidden; height: 21.2993px;">

    <div class="container"
    style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

    -   <span class="votes" style="">
        <a href="#" class="ui-tip vote up selected">Vote Up</a> <span
        class="vote current ui-tip" tip="View Voters"
        replyid="91c70a35-f8bf-4d58-9791-f3faeb6d382c">+1</span>
        <a href="#" class="ui-tip vote down">Vote Down</a> </span>
    -   <span class="ui-presentcontent ui-like ui-tip"
        presentcontents="91c70a35f8bf4d589791f3faeb6d382c|9262536b49a34494802f04dff10424ed|"
        tipcontent="initialmessage"
        contentid="91c70a35-f8bf-4d58-9791-f3faeb6d382c"
        contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
        initialcount="4" initialstate="true" data-readonly="false"
        initialmessage="You and &lt;span class=&quot;who-likes&quot;&gt;3 others&lt;/span&gt; like this"
        format="{toggle} {count}"
        configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
        style=""><span
        class="like-toggle"><a href="#" class="internal-link like-on">Unlike</a></span>
        <span class="like-count who-likes" count="4">4</span></span>
    -   <a href="#" class="internal-link new-reply">Reply</a>
    -   
    -   <a href="#" class="more">More</a>

    </div>

    </div>

    </div>

    <div class="typing-status-wrapper">

    </div>

    <div class="new-replies-wrapper">

    </div>

    </div>

    <div class="newreply">

    </div>

    -   <div class="rendered-content">

        <div class="author" authorprofileurl="/members/digimorf">

        <div class="avatar">

        <a href="/members/digimorf" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/24/55/56/4UJ9DW4IF7NM.jpg-44x44x2.jpg?_=skoPvJtq8/g3Z5q0/xSYxw==" data-border="0px" alt="Digimorf" /></a>

        </div>

        <div class="meta">

        <span class="user-name"> <span
        class="ui-userpresence ui-tip present" userid="245556"
        contentid="d332699e-bc20-475a-9cae-2d7a3e2c034c"
        contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05"
        present="true" presentlabel="Online" notpresentlabel="Offline"
        tip="Online">Online</span>
        <a href="/members/digimorf" class="internal-link view-user-profile">Digimorf</a>
        </span>
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=8a692aaf-63bc-4463-b5d7-6f2dfa10b061" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="6 May 2023 10:29 AM" data-dateutc="2023-05-06T10:29:16.447Z">2 months ago</span></a>
        <span class="parent"> in reply to
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=91c70a35-f8bf-4d58-9791-f3faeb6d382c" class="internal-link view-post navigable ui-tip">dougw</a>
        </span>

        </div>

        </div>

        <div
        class="content full threaded-reply-content user-defined-markup">

        <div class="content">

        Thank you very much, I hope it could be an help and a starting
        point for other projects. 

        <div style="clear:both;">

        </div>

        </div>

        </div>

        <div class="edit-form">

        </div>

        <div class="actions meta edit">

        <div class="navigation-list" maxlinks="1" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null">

        </div>

        </div>

        <div class="actions meta default">

        <div class="navigation-list" maxlinks="3" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null"
        finalized="true" style="overflow: hidden; height: 21.2993px;">

        <div class="container"
        style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

        -   <span class="votes" style="">
            <a href="#" class="ui-tip vote up">Vote Up</a> <span
            class="vote current ui-tip" tip="View Voters"
            replyid="8a692aaf-63bc-4463-b5d7-6f2dfa10b061">0</span>
            <a href="#" class="ui-tip vote down">Vote Down</a> </span>
        -   <span class="ui-presentcontent ui-like ui-tip"
            presentcontents="8a692aaf63bc4463b5d76f2dfa10b061|9262536b49a34494802f04dff10424ed|"
            tipcontent="initialmessage"
            contentid="8a692aaf-63bc-4463-b5d7-6f2dfa10b061"
            contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
            initialcount="3" initialstate="false" data-readonly="false"
            initialmessage="&lt;a href=&quot;/members/ralphjy&quot; class=&quot;internal-link view-user-profile&quot;&gt;ralphjy&lt;/a&gt; and &lt;span class=&quot;who-likes&quot;&gt;2 others&lt;/span&gt; like this"
            format="{toggle} {count}"
            configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
            style=""><span
            class="like-toggle"><a href="#" class="internal-link like-off">Like</a></span>
            <span class="like-count who-likes" count="3">3</span></span>
        -   <a href="#" class="internal-link new-reply">Reply</a>
        -   
        -   
        -   <a href="#" class="more">More</a>

        </div>

        </div>

        </div>

        <div class="typing-status-wrapper">

        </div>

        <div class="new-replies-wrapper">

        </div>

        </div>

        <div class="newreply">

        </div>

-   <div class="rendered-content">

    <div class="author" authorprofileurl="/members/dab">

    <div class="avatar">

    <a href="/members/dab" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/10/43/35/4UEYJN8GSL6S.png-44x44x2.png?_=DYxI/KXozB5/dcOqH5/Pew==" data-border="0px" alt="DAB" /></a>

    </div>

    <div class="meta">

    <span class="user-name"> <span class="ui-userpresence ui-tip"
    userid="104335" contentid="4966e6ad-97f6-4dac-9ea8-919abb220e1d"
    contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="false"
    presentlabel="Online" notpresentlabel="Offline"
    tip="Offline">Offline</span>
    <a href="/members/dab" class="internal-link view-user-profile">DAB</a>
    </span>
    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=ddceb963-2693-4753-ad0d-d4a34ce49e6b" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="6 May 2023 8:27 PM" data-dateutc="2023-05-06T20:27:59.953Z">2 months ago</span></a>

    </div>

    </div>

    <div
    class="content full threaded-reply-content user-defined-markup">

    <div class="content">

    Very good project, well explained.

    <div style="clear:both;">

    </div>

    </div>

    </div>

    <div class="edit-form">

    </div>

    <div class="actions meta edit">

    <div class="navigation-list" maxlinks="1" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null">

    </div>

    </div>

    <div class="actions meta default">

    <div class="navigation-list" maxlinks="3" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null"
    finalized="true" style="overflow: hidden; height: 21.2993px;">

    <div class="container"
    style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

    -   <span class="votes" style="">
        <a href="#" class="ui-tip vote up selected">Vote Up</a> <span
        class="vote current ui-tip" tip="View Voters"
        replyid="ddceb963-2693-4753-ad0d-d4a34ce49e6b">+1</span>
        <a href="#" class="ui-tip vote down">Vote Down</a> </span>
    -   <span class="ui-presentcontent ui-like ui-tip"
        presentcontents="ddceb96326934753ad0dd4a34ce49e6b|9262536b49a34494802f04dff10424ed|"
        tipcontent="initialmessage"
        contentid="ddceb963-2693-4753-ad0d-d4a34ce49e6b"
        contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
        initialcount="4" initialstate="true" data-readonly="false"
        initialmessage="You and &lt;span class=&quot;who-likes&quot;&gt;3 others&lt;/span&gt; like this"
        format="{toggle} {count}"
        configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
        style=""><span
        class="like-toggle"><a href="#" class="internal-link like-on">Unlike</a></span>
        <span class="like-count who-likes" count="4">4</span></span>
    -   <a href="#" class="internal-link new-reply">Reply</a>
    -   
    -   <a href="#" class="more">More</a>

    </div>

    </div>

    </div>

    <div class="typing-status-wrapper">

    </div>

    <div class="new-replies-wrapper">

    </div>

    </div>

    <div class="newreply">

    </div>

    -   <div class="rendered-content">

        <div class="author" authorprofileurl="/members/digimorf">

        <div class="avatar">

        <a href="/members/digimorf" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/24/55/56/4UJ9DW4IF7NM.jpg-44x44x2.jpg?_=skoPvJtq8/g3Z5q0/xSYxw==" data-border="0px" alt="Digimorf" /></a>

        </div>

        <div class="meta">

        <span class="user-name"> <span
        class="ui-userpresence ui-tip present" userid="245556"
        contentid="d332699e-bc20-475a-9cae-2d7a3e2c034c"
        contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05"
        present="true" presentlabel="Online" notpresentlabel="Offline"
        tip="Online">Online</span>
        <a href="/members/digimorf" class="internal-link view-user-profile">Digimorf</a>
        </span>
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=d152ab41-47fd-4b11-ae3b-dbcc8a041548" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="6 May 2023 9:11 PM" data-dateutc="2023-05-06T21:11:15.577Z">2 months ago</span></a>
        <span class="parent"> in reply to
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=ddceb963-2693-4753-ad0d-d4a34ce49e6b" class="internal-link view-post navigable ui-tip">DAB</a>
        </span>

        </div>

        </div>

        <div
        class="content full threaded-reply-content user-defined-markup">

        <div class="content">

        Thank you very much!

        <div style="clear:both;">

        </div>

        </div>

        </div>

        <div class="edit-form">

        </div>

        <div class="actions meta edit">

        <div class="navigation-list" maxlinks="1" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null">

        </div>

        </div>

        <div class="actions meta default">

        <div class="navigation-list" maxlinks="3" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null"
        finalized="true" style="overflow: hidden; height: 21.2993px;">

        <div class="container"
        style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

        -   <span class="votes" style="">
            <a href="#" class="ui-tip vote up selected">Vote Up</a>
            <span class="vote current ui-tip" tip="View Voters"
            replyid="d152ab41-47fd-4b11-ae3b-dbcc8a041548">+1</span>
            <a href="#" class="ui-tip vote down">Vote Down</a> </span>
        -   <span class="ui-presentcontent ui-like ui-tip"
            presentcontents="d152ab4147fd4b11ae3bdbcc8a041548|9262536b49a34494802f04dff10424ed|"
            tipcontent="initialmessage"
            contentid="d152ab41-47fd-4b11-ae3b-dbcc8a041548"
            contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
            initialcount="4" initialstate="false" data-readonly="false"
            initialmessage="&lt;a href=&quot;/members/dougw&quot; class=&quot;internal-link view-user-profile&quot;&gt;dougw&lt;/a&gt; and &lt;span class=&quot;who-likes&quot;&gt;3 others&lt;/span&gt; like this"
            format="{toggle} {count}"
            configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
            style=""><span
            class="like-toggle"><a href="#" class="internal-link like-off">Like</a></span>
            <span class="like-count who-likes" count="4">4</span></span>
        -   <a href="#" class="internal-link new-reply">Reply</a>
        -   
        -   
        -   <a href="#" class="more">More</a>

        </div>

        </div>

        </div>

        <div class="typing-status-wrapper">

        </div>

        <div class="new-replies-wrapper">

        </div>

        </div>

        <div class="newreply">

        </div>

-   <div class="rendered-content">

    <div class="author" authorprofileurl="/members/compact">

    <div class="avatar">

    <a href="/members/compact" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/21/67/50/4UFB9Q7LDYYE.png-44x44x2.png?_=o50HgJrcYMPofECSFgR17Q==" data-border="0px" alt="COMPACT" /></a>

    </div>

    <div class="meta">

    <span class="user-name"> <span class="ui-userpresence ui-tip"
    userid="216750" contentid="7bae85b7-7343-4136-be78-9dfddf955641"
    contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05" present="false"
    presentlabel="Online" notpresentlabel="Offline"
    tip="Offline">Offline</span>
    <a href="/members/compact" class="internal-link view-user-profile">COMPACT</a>
    </span>
    <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=4ee33c24-32ab-48c1-8202-c4221580a5ca" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="19 May 2023 2:45 AM" data-dateutc="2023-05-19T02:45:03.467Z">2 months ago</span></a>

    </div>

    </div>

    <div
    class="content full threaded-reply-content user-defined-markup">

    <div class="content">

    Excellent Stuff - It's good to see that the tradition continues.
    Here's what I did with an AVR8 over 20 years ago. (Apologies for the
    LCD shading artefacts of the camera). Rather than an emulator, the
    game was written purely from observations.

    PSOC Rocks!

    <img src="/resized-image/__size/1280x720/__key/commentfiles/f7d226abd59f475c9d224a79e3f0ec07-0cb28c48-378e-43c0-ab86-fa1a12eee24a/AVRINV.JPG" style="cursor: zoom-in;" alt="AVR8 Invaders" />

    <div style="clear:both;">

    </div>

    </div>

    </div>

    <div class="edit-form">

    </div>

    <div class="actions meta edit">

    <div class="navigation-list" maxlinks="1" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null">

    </div>

    </div>

    <div class="actions meta default">

    <div class="navigation-list" maxlinks="3" direction="horizontal"
    reflow-wait-check-count="0" reflow-wait-timeout="null"
    finalized="true" style="overflow: hidden; height: 21.2993px;">

    <div class="container"
    style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

    -   <span class="votes" style="">
        <a href="#" class="ui-tip vote up">Vote Up</a> <span
        class="vote current ui-tip" tip="View Voters"
        replyid="4ee33c24-32ab-48c1-8202-c4221580a5ca">0</span>
        <a href="#" class="ui-tip vote down">Vote Down</a> </span>
    -   <span class="ui-presentcontent ui-like ui-tip"
        presentcontents="4ee33c2432ab48c18202c4221580a5ca|9262536b49a34494802f04dff10424ed|"
        tipcontent="initialmessage"
        contentid="4ee33c24-32ab-48c1-8202-c4221580a5ca"
        contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
        initialcount="5" initialstate="true" data-readonly="false"
        initialmessage="You and &lt;span class=&quot;who-likes&quot;&gt;4 others&lt;/span&gt; like this"
        format="{toggle} {count}"
        configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
        style=""><span
        class="like-toggle"><a href="#" class="internal-link like-on">Unlike</a></span>
        <span class="like-count who-likes" count="5">5</span></span>
    -   <a href="#" class="internal-link new-reply">Reply</a>
    -   
    -   <a href="#" class="more">More</a>

    </div>

    </div>

    </div>

    <div class="typing-status-wrapper">

    </div>

    <div class="new-replies-wrapper">

    </div>

    </div>

    <div class="newreply">

    </div>

    -   <div class="rendered-content">

        <div class="author" authorprofileurl="/members/dougw">

        <div class="avatar">

        <a href="/members/dougw" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/01/66/37/4UEYEAB88T8D.png-44x44x2.png?_=s+tKfYvQfvPoTdTDvv+m6g==" data-border="0px" alt="dougw" /></a>

        </div>

        <div class="meta">

        <span class="user-name"> <span class="ui-userpresence ui-tip"
        userid="16637" contentid="193ac243-4c81-4a24-b466-35a2210baec1"
        contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05"
        present="false" presentlabel="Online" notpresentlabel="Offline"
        tip="Offline">Offline</span>
        <a href="/members/dougw" class="internal-link view-user-profile">dougw</a>
        </span>
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=3bac6509-6445-4c65-82f5-bc06b7843e4f" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="19 May 2023 3:56 AM" data-dateutc="2023-05-19T03:56:56.080Z">2 months ago</span></a>
        <span class="parent"> in reply to
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=4ee33c24-32ab-48c1-8202-c4221580a5ca" class="internal-link view-post navigable ui-tip">COMPACT</a>
        </span>

        </div>

        </div>

        <div
        class="content full threaded-reply-content user-defined-markup">

        <div class="content">

        Nice job.

        <div style="clear:both;">

        </div>

        </div>

        </div>

        <div class="edit-form">

        </div>

        <div class="actions meta edit">

        <div class="navigation-list" maxlinks="1" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null">

        </div>

        </div>

        <div class="actions meta default">

        <div class="navigation-list" maxlinks="3" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null"
        finalized="true" style="overflow: hidden; height: 21.2993px;">

        <div class="container"
        style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

        -   <span class="votes" style="">
            <a href="#" class="ui-tip vote up">Vote Up</a> <span
            class="vote current ui-tip" tip="View Voters"
            replyid="3bac6509-6445-4c65-82f5-bc06b7843e4f">0</span>
            <a href="#" class="ui-tip vote down">Vote Down</a> </span>
        -   <span class="ui-presentcontent ui-like ui-tip"
            presentcontents="3bac650964454c6582f5bc06b7843e4f|9262536b49a34494802f04dff10424ed|"
            tipcontent="initialmessage"
            contentid="3bac6509-6445-4c65-82f5-bc06b7843e4f"
            contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
            initialcount="2" initialstate="false" data-readonly="false"
            initialmessage="&lt;a href=&quot;/members/ralphjy&quot; class=&quot;internal-link view-user-profile&quot;&gt;ralphjy&lt;/a&gt; and &lt;span class=&quot;who-likes&quot;&gt;1 other&lt;/span&gt; like this"
            format="{toggle} {count}"
            configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
            style=""><span
            class="like-toggle"><a href="#" class="internal-link like-off">Like</a></span>
            <span class="like-count who-likes" count="2">2</span></span>
        -   <a href="#" class="internal-link new-reply">Reply</a>
        -   
        -   <a href="#" class="more">More</a>

        </div>

        </div>

        </div>

        <div class="typing-status-wrapper">

        </div>

        <div class="new-replies-wrapper">

        </div>

        </div>

        <div class="newreply">

        </div>

    -   <div class="rendered-content">

        <div class="author" authorprofileurl="/members/digimorf">

        <div class="avatar">

        <a href="/members/digimorf" class="internal-link view-user-profile"><img src="https://community-storage.element14.com/communityserver-components-imagefileviewer/communityserver/components/avatars/00/00/24/55/56/4UJ9DW4IF7NM.jpg-44x44x2.jpg?_=skoPvJtq8/g3Z5q0/xSYxw==" data-border="0px" alt="Digimorf" /></a>

        </div>

        <div class="meta">

        <span class="user-name"> <span
        class="ui-userpresence ui-tip present" userid="245556"
        contentid="d332699e-bc20-475a-9cae-2d7a3e2c034c"
        contenttypeid="e9ed4118-60ed-4f2b-a026-5705b8793d05"
        present="true" presentlabel="Online" notpresentlabel="Offline"
        tip="Online">Online</span>
        <a href="/members/digimorf" class="internal-link view-user-profile">Digimorf</a>
        </span>
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=d9291b22-b953-44f8-aa5d-c4ae872c1bd7" class="internal-link view-post navigable reply-permalink"><span class="ui-agodate ui-tip" data-tip="19 May 2023 8:57 AM" data-dateutc="2023-05-19T08:57:53.027Z">2 months ago</span></a>
        <span class="parent"> in reply to
        <a href="https://community.element14.com/challenges-projects/design-challenges/at-the-core-design-challenge/b/blog/posts/atcdc-digimorf-sega-sg-1000-emulator-on-psoc62s4-pioneer-kit-everything-unveiled?CommentId=4ee33c24-32ab-48c1-8202-c4221580a5ca" class="internal-link view-post navigable ui-tip">COMPACT</a>
        </span>

        </div>

        </div>

        <div
        class="content full threaded-reply-content user-defined-markup">

        <div class="content">

        Space Invaders is always in our heart <span
        class="emoticon ui-tip"
        title="Heart eyes"><img src="https://community.element14.com/cfs-file/__key/system/emoji/1f60d.svg" style="max-height: 32px;max-width: 32px;" alt="Heart eyes" /></span>

        Good job! 

        <div style="clear:both;">

        </div>

        </div>

        </div>

        <div class="edit-form">

        </div>

        <div class="actions meta edit">

        <div class="navigation-list" maxlinks="1" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null">

        </div>

        </div>

        <div class="actions meta default">

        <div class="navigation-list" maxlinks="3" direction="horizontal"
        reflow-wait-check-count="0" reflow-wait-timeout="null"
        finalized="true" style="overflow: hidden; height: 21.2993px;">

        <div class="container"
        style="overflow: auto hidden; height: 41.2993px; visibility: visible;">

        -   <span class="votes" style="">
            <a href="#" class="ui-tip vote up">Vote Up</a> <span
            class="vote current ui-tip" tip="View Voters"
            replyid="d9291b22-b953-44f8-aa5d-c4ae872c1bd7">0</span>
            <a href="#" class="ui-tip vote down">Vote Down</a> </span>
        -   <span class="ui-presentcontent ui-like ui-tip"
            presentcontents="d9291b22b95344f8aa5dc4ae872c1bd7|9262536b49a34494802f04dff10424ed|"
            tipcontent="initialmessage"
            contentid="d9291b22-b953-44f8-aa5d-c4ae872c1bd7"
            contenttypeid="9262536b-49a3-4494-802f-04dff10424ed"
            initialcount="2" initialstate="false" data-readonly="false"
            initialmessage="&lt;a href=&quot;/members/ralphjy&quot; class=&quot;internal-link view-user-profile&quot;&gt;ralphjy&lt;/a&gt; and &lt;span class=&quot;who-likes&quot;&gt;1 other&lt;/span&gt; like this"
            format="{toggle} {count}"
            configuration="Format=%7Btoggle%7D%20%7Bcount%7D&amp;IncludeTip=true"
            style=""><span
            class="like-toggle"><a href="#" class="internal-link like-off">Like</a></span>
            <span class="like-count who-likes" count="2">2</span></span>
        -   <a href="#" class="internal-link new-reply">Reply</a>
        -   
        -   
        -   <a href="#" class="more">More</a>

        </div>

        </div>

        </div>

        <div class="typing-status-wrapper">

        </div>

        <div class="new-replies-wrapper">

        </div>

        </div>

        <div class="newreply">

        </div>

</div>

</div>

<div id="fragment-7298_editorWrapperWrapper" style="display:none">

<div id="mceu_24"
class="mce-tinymce mce-container mce-panel mce-editor-submittable mce-editor-desktop mce-editor-blur mce-submitpanel-narrow"
hidefocus="1" tabindex="-1" role="application"
style="visibility: hidden; border-width: 1px; height: auto; overflow: visible;">

<div id="mceu_24-body" class="mce-container-body mce-stack-layout">

<div id="mceu_25"
class="mce-container mce-menubar mce-toolbar mce-stack-layout-item mce-first"
role="menubar" style="border-width: 0px 0px 1px; display: none;">

<div id="mceu_25-body" class="mce-container-body mce-flow-layout">

<div id="mceu_26"
class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-first mce-btn-has-text"
tabindex="-1" aria-labelledby="mceu_26" role="menuitem"
aria-haspopup="true">

<span class="mce-txt">Edit</span>

</div>

<div id="mceu_27"
class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-btn-has-text"
tabindex="-1" aria-labelledby="mceu_27" role="menuitem"
aria-haspopup="true">

<span class="mce-txt">Insert</span>

</div>

<div id="mceu_28"
class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-btn-has-text"
tabindex="-1" aria-labelledby="mceu_28" role="menuitem"
aria-haspopup="true">

<span class="mce-txt">Format</span>

</div>

<div id="mceu_29"
class="mce-widget mce-btn mce-menubtn mce-flow-layout-item mce-last mce-btn-has-text"
tabindex="-1" aria-labelledby="mceu_29" role="menuitem"
aria-haspopup="true">

<span class="mce-txt">Tools</span>

</div>

</div>

</div>

<div id="mceu_33"
class="mce-edit-area mce-container mce-panel mce-stack-layout-item mce-last"
hidefocus="1" tabindex="-1" role="group"
style="border-width: 0px 0px 1px; position: relative;">

<div style="position: absolute; top: 0px; left: 0px;">

<div class="mceDragDropShade"
style="position: absolute; display: none; left: 0px; top: 0px;">

</div>

<div class="mceDragDropMessage"
style="position: absolute; display: none; left: 0px; top: 0px;">

</div>

</div>

</div>

<div id="mceu_30"
class="mce-toolbar-grp mce-container mce-panel mce-stack-layout-item"
hidefocus="1" tabindex="-1" role="group">

<div id="mceu_30-body" class="mce-container-body mce-stack-layout">

<div id="mceu_31"
class="mce-container mce-toolbar mce-stack-layout-item mce-first mce-last"
role="toolbar">

<div id="mceu_31-body" class="mce-container-body mce-flow-layout">

<div id="mceu_32"
class="mce-container mce-flow-layout-item mce-first mce-last mce-btn-group"
role="group" style="width: 100%;">

<div id="mceu_32-body" style="white-space: normal;">

<div id="mceu_17"
class="mce-widget mce-btn mce-menubtn mce-first mce-btn-has-text"
tabindex="-1" aria-labelledby="mceu_17" role="button"
aria-haspopup="true">

<span class="mce-txt">Edit</span>

</div>

<div id="mceu_18"
class="mce-widget mce-btn mce-menubtn mce-btn-has-text" tabindex="-1"
aria-labelledby="mceu_18" role="button" aria-haspopup="true">

<span class="mce-txt">Insert</span>

</div>

<div id="mceu_19"
class="mce-widget mce-btn mce-menubtn mce-btn-has-text" tabindex="-1"
aria-labelledby="mceu_19" role="button" aria-haspopup="true">

<span class="mce-txt">Format</span>

</div>

<div id="mceu_20"
class="mce-widget mce-btn mce-menubtn mce-btn-has-text" tabindex="-1"
aria-labelledby="mceu_20" role="button" aria-haspopup="true">

<span class="mce-txt">Tools</span>

</div>

<div id="mceu_21" class="mce-widget mce-btn" tabindex="-1"
aria-labelledby="mceu_21" role="button" aria-label="Bullet list">

</div>

<div id="mceu_22" class="mce-widget mce-btn" tabindex="-1"
aria-labelledby="mceu_22" role="button" aria-label="Numbered list">

</div>

<div id="mceu_23" class="mce-widget mce-submitpanel mce-last"
role="presentation" aria-haspopup="false">

<div id="mceu_23_send"
class="mce-widget mce-btn mce-primary mce-btn-has-text mce-submitpanel-button"
tabindex="0" role="button">

<span class="mce-txt">Reply</span>

</div>

</div>

</div>

</div>

</div>

</div>

</div>

</div>

</div>

<div
style="position: absolute; z-index: -1; height: 1px; width: 1px; overflow: hidden;">

</div>

</div>

</div>

</div>

<div class="content-fragment-footer">

</div>

</div>

</div>
