<p><span style="font-size: 150%;"><strong>SEGA SG-1000 Emulator on PSoC62S4 Pioneer kit</strong></span></p>
<p><span style="font-size: inherit;">Hi everyone!</span></p>
<p><span style="font-size: inherit;">Here we are at the conclusion of this adventure in the world of Infineon with my project for this challenge.</span></p>
<p><span style="font-size: inherit;">In these two months, I have tried to share all the salient steps in the creation of a SEGA SG-1000 console emulator. Since the purpose of this challenge was to push this system to its limits, I felt it was important to share the steps taken by a developer using a PSoC6 for the first time for the realization of a multi-disciplinary project: generation of a video signal, generation of audio in real-time, implementation of a virtual machine, all trying to use the resources (limited for this type of project) of this microcontroller.</span></p>
<p><span style="font-size: inherit;">This final post wants to officially be a more exhaustive description of this project.</span></p>
<p><span style="font-size: 150%;"><strong>1.0 - Introduction</strong></span></p>
<p><img class="align-right" style="float: right; max-height: 159px; max-width: 329px;" alt="SEGA SG-1000" src="/resized-image/__size/658x318/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/variation_5F00_5F00_5F00_img_5F00_5F00_5F00_2.jpg" />The idea for this project came to me while looking at this PSoC62 Pioneer Kit. On the PCB it features a "Touchpad" which reminded me of a Joypad. The challenge launched by Element14 together with Infineon proposed reaching the limits of this system, trying to exploit the characteristics of the on-board microcontroller.</p>
<p>As I explained in my posts during this challenge, I'm a retro enthusiast and have written several emulators in the past. My favorite system, which was the first one I had in my youth, is the SEGA SG-1000 console. So I wanted to rewrite one of the SEGA SG-1000 emulators that I have written and port it to the board in question.</p>
<p><span style="font-size: 150%;"><strong>1.1 - Difficulties and limits</strong></span></p>
<p><span style="font-size: inherit;"><img class="align-right" style="float: right; max-height: 360px; max-width: 240px;" alt="SEGA SG-100 mainbord" src="/resized-image/__size/480x720/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/6661.Sega_5F00_5F00_5F00_SG_5F00_2D00_5F00_1000_5F00_2D00_5F00_v1_5F00_2D00_5F00_PCB.jpg" />Emulating a game console is a rather complex, resource-intensive project. I thought it was a perfect candidate to push the PSoC62 Pioneer Kit to the limit.</span></p>
<p><span style="font-size: inherit;">I can say that an emulator is divided into two main areas: Virtual Machine and Hardware Input/output. The microcontroller on board is a dual-core Arm Cortex M4/M0+, so I can divide these areas into one core each.&nbsp;</span></p>
<p><span style="font-size: inherit;">The two cores are configurable through the ModusToolbox ecosystem, so it is possible to decide how to divide the resources between the two cores, and at what speed to set the clocks. For me, it was a nice surprise to see the high customization and ease of use of ModusToolbox.</span></p>
<p><span style="font-size: inherit;">In the end, I decided to set the CM4 core at 150MHz and the CM0+ at 75MHz. I left the emulation of the Input/Output hardware components at the CM0+ core, and the execution of the virtual machine at the CM4 core. That means that the cm0+ will generate the VGA video and the audio signals, it will handle also the Capsense joypad, and it will synchronize the execution of the virtual machine in the CM4 core.</span></p>
<p><span style="font-size: inherit;"><img class="align-left" style="float: left; max-height: 360px; max-width: 240px;" alt="CY8CKIT-062S4 PSoC 62S4 pioneer kit" src="/resized-image/__size/480x720/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/7206.CY8CKIT.jpg" />Another aspect that is difficult to manage is the division of the SRAM between the two cores. The original hardware to be emulated has 2Kb of RAM, 16Kb of special VRAM used by the Video Display Processor, and last but not least, the memory used by the game cartridge.</span></p>
<p>The Infineon PSoC 6 MCU in the Pioneer Kit is the CY8C6244LQI-S4D92, which has 128Kb SRAM and 256Kb flash ROM. Apparently, everything should work perfectly, but it is not so obvious. The microcontroller must generate a video signal, which for the console to be emulated, must have a resolution of 256x192 pixels, therefore, unless it were possible to generate all the graphics on the fly, a frame buffer of about 49Kb is needed to host a video frame.</p>
<p>Then, in the same way, the audio generation relies on a buffer that contains all the samples to stream. in this case, we need one of around 1Kb (more on this later). Don't forget that the source code of the emulator will create variables, temporary buffers, etc. in a stack, so you need to give a reasonable space to stack and heap.</p>
<p><img class="align-right" style="float: right; height: 223px; max-height: 223px; max-width: 331px;" alt="VGA timings" height="223" src="/resized-image/__size/662x446/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/3000.VGAtimings.png" width="330" />Speed is also crucial, since the cm0+ core must be able to generate a standard VGA signal, and the CM4 core must be able to perform a number of calculations to emulate a 3.58MHz CPU, a 10.74MHz VDP, and a Programmable Sound Generator (PSG) at 3.58MHz, in order to display graphics, sound and manage a Joypad in real-time.</p>
<p><span style="font-size: 150%;"><strong></strong></span>Two months were available for this project, so I had to accept a compromise between the quality and usability of the emulator.&nbsp;</p>
<p><span style="font-size: 150%;"><strong>1.2 -&nbsp;The emulator and the game</strong></span></p>
<p><span style="font-size: inherit;">Two months were available for this project, so I had to accept a compromise between the quality and usability of the emulator.&nbsp;</span></p>
<p><span style="font-size: inherit;">The emulator will have to perform different tasks divided into time intervals so that it is possible to simulate hardware composed of several units that work in multitasking. Since the emulator is synchronized with a video signal, the time intervals will be equal to a video scanline. Then, the Z80 virtual CPU will run for a series of cycles equal to the time interval of an emulated video scanline, and then the graphics and sound for that scanline will be rendered.</span></p>
<p><span style="font-size: inherit;">I won't go too much into the details of the emulation, but this means that c</span><span style="font-size: inherit;">onsidering the VGA standard timings:</span></p>
<h2 id="mcetoc_1gvbq71910">Horizontal timing (line)</h2>
<p><span style="font-size: inherit;"><span>Polarity of horizontal sync pulse is negative.</span></span></p>
<table style="width: 320px;">
<tbody>
<tr>
<td>Scanline part</td>
<td>Pixels</td>
<td>Time [&micro;s]</td>
</tr>
<tr>
<td>Visible area</td>
<td>640</td>
<td align="right">25.422045680238</td>
</tr>
<tr>
<td>Front porch</td>
<td>16</td>
<td align="right">0.63555114200596</td>
</tr>
<tr>
<td>Sync pulse</td>
<td>96</td>
<td align="right">3.8133068520357</td>
</tr>
<tr>
<td>Back porch</td>
<td>48</td>
<td align="right">1.9066534260179</td>
</tr>
<tr>
<td>Whole line</td>
<td>800</td>
<td align="right">31.777557100298</td>
</tr>
</tbody>
</table>
<h2 id="mcetoc_1gvbq71911">Vertical timing (frame)</h2>
<p><span style="font-size: inherit;"><span>Polarity of vertical sync pulse is negative.</span></span></p>
<table style="width: 320px;">
<tbody>
<tr>
<td>Frame part</td>
<td>Lines</td>
<td>Time [ms]</td>
</tr>
<tr>
<td>Visible area</td>
<td>480</td>
<td align="right">15.253227408143</td>
</tr>
<tr>
<td>Front porch</td>
<td>10</td>
<td align="right">0.31777557100298</td>
</tr>
<tr>
<td>Sync pulse</td>
<td>2</td>
<td align="right">0.063555114200596</td>
</tr>
<tr>
<td>Back porch</td>
<td>33</td>
<td align="right">1.0486593843098</td>
</tr>
<tr>
<td>Whole frame</td>
<td>525</td>
<td align="right">16.683217477656</td>
</tr>
</tbody>
</table>
<p><span style="font-size: inherit;">I need to render a complete scanline (CPU + VDP + PSG + IO) in 31.777 us. It may seem like an impossible mission, but there are tricks that we can use to do it. So this should be the final result of the game that I want to run on this emulator:</span></p>
<p><span style="font-size: inherit;">[View:https://youtube.com/watch?v=Of_kFSOZSxo:640:360]</span></p>
<p><span style="font-size: inherit;">And this is the same game emulated by the&nbsp;PSoC62S4 Pioneer kit:</span></p>
<p><span style="font-size: inherit;">[View:https://youtu.be/izYSrum5AMo:854:480]</span></p>
<p><span style="font-size: inherit;">I decided to use "Space Invaders" for one reason only: The Capsense area present on the Pioneer Kit has 1 axis direction slider and two buttons, which is perfect for this game. I used the slider as a rotary encoder, and the two buttons as normal fire buttons.</span></p>
<p><span style="font-size: 150%;"><strong>2.0 - Architecture</strong></span></p>
<p><span style="font-size: inherit;">Let's examine the architecture of this project in the following schematic:</span></p>
<p><span style="font-size: inherit;"><img style="display: block; height: 493px; margin-left: auto; margin-right: auto; max-height: 493px; max-width: 1032px;" alt="SEGA SG-1000 emulator on PSoC62 diagram" height="426" src="/resized-image/__size/2064x986/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/Application.png" width="1031" /></span></p>
<p><span style="font-size: inherit;">This schematic shows clearly that the hardware interface of the emulator is handled by the CM0+ core, while the software emulation of the console is left to the CM4 core. In ModusToolbox the system is configured for using the external 24MHz clock on the Pioneer Kit that offers a better accuracy and stability of the operating frequencies for the cores.</span></p>
<p><span style="font-size: inherit;"><img style="display: block; height: 589px; margin-left: auto; margin-right: auto; max-height: 589px; max-width: 925px;" alt=" " height="530" src="/resized-image/__size/1850x1178/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/8304.clocking_2D00_system.jpg" width="925" /></span></p>
<p><span style="font-size: inherit;">The routing shown in the system schematic of the ModusToolbox Device Configurator sets the clock for the CM0+ core to 75MHz, and the clock of the CM4 to 150MHz. So the minimum time interval of the Peripheral Clock is 13.33 ns, enough for the timers and the DMA functionality.</span></p>
<p><span style="font-size: 150%;"><strong>2.1 - Hardware side on CM0+</strong></span></p>
<p><span style="font-size: inherit;">The CM0+ core&nbsp;</span><span style="font-size: inherit;">takes care of driving the hardware interfaces for the emulator, such as Video, Audio, and I/O. So I started first with the development of the VGA video driver since it's the unit that rules all the emulation.</span></p>
<p><span style="font-size: 150%;"><strong><span>2.1.1 - VGA&nbsp;DAC</span></strong></span></p>
<p>As I mentioned earlier, the video signal generated complies with the VGA standard, therefore with a vertical frequency of 60Hz and a horizontal one of 31.468 kHz. The vertical resolution maintains the standard of 525 total lines for a visible vertical area of 480 lines, while for the horizontal resolution, we will use a "Pixel Clock" which we will configure to obtain a resolution suitable for the emulation.</p>
<p>The Pioneer Kit does not have a VGA video output, but building it is very simple and requires a few resistors, a DE15 socket connector, some jumper wires, and a solderless breadboard. All components can be easily found on eBay or Amazon, but I wanted to bring your attention to this solderless VGA socket:</p>
<p><a title="Solderless VGA plug" href="https://www.amazon.com/ANMBEST-Solderless-Connector-Terminal-Compatible/dp/B08YJKG1HV/ref=sr_1_4?crid=320HUR1FEDR0V&amp;keywords=vga%2Bplug&amp;qid=1683023856&amp;sprefix=vga%2Bplu%2Caps%2C176&amp;sr=8-4&amp;th=1" rel="noopener noreferrer" target="_blank">Solderless VGA plug</a></p>
<p><img style="display: block; height: 519px; margin-left: auto; margin-right: auto; max-height: 519px; max-width: 652px;" alt="Solderless breadboard RGB VGA DAC" height="519" src="/resized-image/__size/1304x1038/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/8132.RGBDAC.png" width="651" /></p>
<p>This little circuit is a triple analog-to-digital converter that takes the values of Red, Green and Blue and converts them into voltages using simple resistors. The proposed circuit offers a resolution of 8 shades of Red and Green, while only 4 for Blue, which however are sufficient for the visual impact. This division is called "color depth" and is usually referred to as "BPP" or bits per plane.<br />The signal will therefore be rgb332 and will allow us to obtain 256 colors and the following palette.</p>
<p><img style="display: block; margin-left: auto; margin-right: auto; max-height: 247px; max-width: 438px;" alt="" src="/resized-image/__size/876x494/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/palette_2D00_rgb332.png" /></p>
<p>This color depth is very useful because it fits right into a byte, making it easy to match 1 pixel to 1 byte. Here we understand why to match an entire GPIO port to the used RGB signals.</p>
<p><img style="display: block; margin-left: auto; margin-right: auto; max-height: 532px; max-width: 600px;" alt=" " src="/resized-image/__size/1200x1064/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/4812.PioneerKitVGA.png" /></p>
<p></p>
<p><span style="font-size: 150%;"><strong><span>2.1.2 -&nbsp;Audio&nbsp;Signal</span></strong></span></p>
<p>The audio signal is generated by the DAC embedded into the PSoC62, and it is fed by the virtual PSG of the emulator. The simplest hardware you need to plug your Pioneer Kit is a bipolar shielded wire for audio signals and a 3.5mm stereo jack plug.</p>
<p><img style="display: block; height: 193px; margin-left: auto; margin-right: auto; max-height: 193px; max-width: 453px;" alt=" " height="96" src="/resized-image/__size/906x386/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/5483.audio.png" width="452" /></p>
<p>The resolution of this DAC is 12 bits, this means that the signal coming out of the assigned GPIO can assume 2^12 values to modulate a voltage between 0 and 3.3v (typically).</p>
<p><img style="display: block; margin-left: auto; margin-right: auto; max-height: 532px; max-width: 600px;" alt=" " src="/resized-image/__size/1200x1064/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/1263.PioneerKitAudio.png" /></p>
<p><span style="font-size: 150%;"><strong><span>2.1.3 -&nbsp;Capsense Joypad</span></strong></span></p>
<p>The Pioneer Kit has some capacitive areas on board that are connected to specific pins of the PSoC62. These pins can be dri0ven with the Capsense technology which simplifies the configuration and the building of capacitive devices. In this specific project, this interface reminds us of a one-axis joypad with two fire buttons. So, it's possible to use it directly as an input device by configuring it through the Capsense configurator.</p>
<p><img style="display: block; height: 381px; margin-left: auto; margin-right: auto; max-height: 381px; max-width: 965px;" alt=" " height="348" src="/resized-image/__size/1930x762/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/7120.CapsenseJoypad.png" width="964" /></p>
<p><span style="font-size: 150%;"><strong>2.2 -&nbsp;Software side on CM0+</strong></span></p>
<p>The project was derived from the "Dual-CPU Empty PSoC6 App" example project within ModusToolbox. As shown above, I modified the clock system, and the Board Support Package, and from there I added various video, audio, and joypad devices.</p>
<p><span style="font-size: 150%;"><strong><span>2.2.1 - VGA&nbsp;Driver</span></strong></span></p>
<p>The VGA driver is capable of generating a video signal that complies with the VGA standard, so it should be compatible with most TV/Monitors that can connect to a PC.<br />The generated signal consists of the generation of 60 frames per second composed of a sequence of 525 scanlines. The scanlines are divided into groups or areas: Vertical Sync, the Back Porch which precedes the actual image, the Active Area with the image made up of 480 lines of pixels, and finally the Front Porch which closes the image.</p>
<p><img style="display: block; height: 965px; margin-left: auto; margin-right: auto; max-height: 965px; max-width: 859px;" alt=" " height="965" src="/resized-image/__size/1718x1930/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/2352.VGA_5F00_VideoDriver_5F00_Frame.png" width="858" />For more details, you can check my posts on this blog, but here I can say that all the scanlines are composed of the two sync signals (Horizontal and Vertical), ad the three color signals (R, G, B). In all scanlines, there is a horizontal sync pulse, and the vertical sync signal is high, except for the vertical sync period, which presents two lines with a low state for the vertical sync.&nbsp;</p>
<p>The color signals are at a blank level (black) in all scanlines except in the Active Area, where they are modulated thanks to the data coming from the frame buffer and combined together by the resistor DAC.</p>
<p>To send the correct line, the driver counts the scanlines within an interrupt service routine (ISR), which is triggered at the end of each line via an IRQ event. In order to execute a line with the correct characteristics, a Counter with a PWM signal is used, which integrates an impulse, in this case negative, corresponding to the horizontal synchronism of the standard VGA signal. In addition, two "Counter Compare" events are set along the line to activate the RGB signals and therefore the streaming of the pixels at regular intervals, according to a "Pixel Counter".</p>
<p>We have seen that the graphics resolution of the SEGA SG-1000 is 256x192, and the resolution for the VGA signal is 640x480. We can double the original resolution and get a bigger picture. For horizontal resolution, we just need to "slow down" the PIXEL Clock, while for vertical resolution we can transmit the same line twice.</p>
<p>We have seen that the graphics resolution of the SEGA SG-1000 is 256x192, and the resolution for the VGA signal is 640x480. We can double the original resolution and get a bigger picture. For horizontal resolution, we just need to "slow down" the PIXEL Clock, while for vertical resolution we can transmit the same line twice. Since we are emulating a console that typically used to be hooked up to a CRT television, we can transmit one line, and turn off the next line to create the typical effect of scanlines. We also need this trick to free up access to the frame buffer in RAM while the CM4 core accesses it during the rendering phase.</p>
<p>Let's examine how this system works:</p>
<p><img style="display: block; height: 563px; margin-left: auto; margin-right: auto; max-height: 563px; max-width: 948px;" alt=" " height="563" src="/resized-image/__size/1896x1126/__key/communityserver-blogs-components-weblogfiles/00-00-00-03-92/7142.VGA_5F00_VideoDriver_5F00_Scanline.png" width="947" /></p>
<p>I extracted a sample of Active Area scanlines. As I explained before we have a scanline containing the pixels of the graphics, and subsequently an empty scanline (black) where no pixels are transmitted but the CM4 core has free access to the SRAM for rendering the graphics.</p>
<p>That said, let's analyze the active scanline, here's what happens.</p>
<p>The signal begins with the negative horizontal sync pulse, lasting 3.81us. For this purpose, I used a TCPWM counter which I set with the "Device Manager".</p>
<p><img style="display: block; height: 747px; margin-left: auto; margin-right: auto; max-height: 747px; max-width: 465px;" alt="" height="747" src="/resized-image/__size/930x1494/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/TCPWM.png" width="464" /></p>
<p>Calculating the appropriate values is simple because the timer counts the time at each clock cycle, which for peripherals is 75Mhz. So each cycle lasts 13.33ns, and so to get the 3.81us pulse, I have to set the Counter Compare 0 to 3810ns /13.33ns = 285.82 cycles, so 286. Along the scanline, this is the HSync area.&nbsp;</p>
<p>After that, there is the Back Porch that ends at the "Counter Compare 1", and at this point, the streaming of the pixels starts. So at the compare 1 match, a second TCPWM is started. This one is the Pixel Clock, which runs continuously until all pixels of the scanline are transmitted.</p>
<p><img style="display: block; height: 766px; margin-left: auto; margin-right: auto; max-height: 766px; max-width: 465px;" alt="" height="533" src="/resized-image/__size/930x1532/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/PixelCounter.png" width="464" /></p>
<p>Here the Pixel Counter counts 7 cycles, and then it triggers a DMA transfer, which copies one byte from the frame buffer to the GPIO port connected to the RGB DAC. Here 7&nbsp;cycles (0..6) correspond to 93.31ns, which visually corresponds to the width of one pixel. Every DMA transfer decrements the number of pixels to stream. This number is set in the DMA configuration.</p>
<p><img style="display: block; height: 550px; margin-left: auto; margin-right: auto; max-height: 550px; max-width: 471px;" alt="" height="574" src="/resized-image/__size/942x1100/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/DMAC.png" width="470" /></p>
<p>As soon as the "Number of data elements to transfer" reaches 0 the&nbsp;Pixel Counter is stopped. Why 257 since the horizontal resolution is 256? When the GPIO port receives one byte from the DMA, sets the pins 0..7 to the state of all bits of the byte.</p>
<p><img style="display: block; height: 422px; margin-left: auto; margin-right: auto; max-height: 422px; max-width: 431px;" alt="" height="422" src="/resized-image/__size/862x844/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/GPIO.png" width="430" /></p>
<p>The pins of the port remain in their state until a new transfer&nbsp;from the DMA. That's why a pixel is 7 cycles wide, and that's why pixel 257 is used to stop the RGB signal because it's always set to black. When&nbsp;the first TCPWM reaches the end at cycle 2384 (2384 * 13.33ns = 31.778us) the scanline ends, the IRQ event fires, and the ISR is called.&nbsp;</p>
<p>Within the ISR the scanlines are counted, and as we saw in the schematic, the frame is composed.</p>
<p>After a scanline is shown the DMA is disabled, so the Pixel Counter won't trigger a memory transfer and the SRAM is free for any operation by the CM4.&nbsp;So the ISR sends a message to the CM4 core containing the number of the next scanline to render, so for the next scanline pixels data are ready to be streamed to GPIOs.</p>
<p><span style="font-size: 150%;"><strong>2.2.2 -&nbsp;Audio Driver</strong></span></p>
<p>Generating a digital audio signal is pretty simple and has several ways to do it. We are fortunate that the PSoC6 has a Digital Analog Converter (DAC) which can be configured to continuously convert a digital value into an analog voltage level in real-time. To obtain a sound we just need to vary this level over time, in order to create oscillations. This signal applied to an amplifier and a speaker, is output as a sound.</p>
<p>To generate a sound with the DAC we need a buffer that contains a series of data to be sent in sequence, and a counter/timer to send the data at regular intervals to the DAC. The rate at which data is sent determines the sample rate of the sound. To reproduce previously recorded sounds, we must necessarily use the same sampling rate, but since the sound is generated by the same application, i.e. the emulator, we have the freedom to decide the frequency ourselves.</p>
<p>Since we need to synchronize audio to video, we can take advantage of the video driver to send an audio sample to each scanline of the screen to get as close as possible to a faithful reproduction of the audio generated by the PSG.</p>
<p>I won't go into detail about the SN76489 PSG emulation, what is important to know is that the code renders an audio sample for each scanline in sequence at each call. Thanks to ModusToolbox, the basic configuration of the DAC is very easy since we need only to send data to it.</p>
<p><img style="display: block; height: 781px; margin-left: auto; margin-right: auto; max-height: 781px; max-width: 505px;" alt="" height="684" src="/resized-image/__size/1010x1562/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/dac.png" width="504" /></p>
<p>for sending the audio samples to the DAC, we just have to use the appropriate function at the beginning of each scanline within the ISR of the VGA driver.</p>
<p>[embed:dc8ab71f-3b98-42d9-b0f6-e21e02a0f8e2:8712f3cb-ae1c-4c2c-84b8-57c82dfa294f:type=c_cpp&text=%2F%2F%0A%20%20%20%20Cy_CTDAC_SetValue%28audio_dac_HW%2C%20gAudioBufferFront%5BgVGAScanline%5D%29%3B%0A%2F%2F]</p>
<p>As already mentioned, the audio samples are read from a buffer, which frame by frame is exchanged with a secondary buffer on which the emulator has prepared new samples to be sent to the DAC. This technique is called double buffering.</p>
<p>[embed:dc8ab71f-3b98-42d9-b0f6-e21e02a0f8e2:612b7960-b502-4403-92af-a508bbaf430e:type=c_cpp&text=%2F%2F%20%20%20%20%0A%20%20%20%20%20%20tmpPtr%20%3D%20gAudioBufferFront%3B%0A%20%20%20%20%20%20gAudioBufferFront%20%3D%20gAudioBufferBack%3B%0A%20%20%20%20%20%20gAudioBufferBack%20%3D%20tmpPtr%3B%0A%2F%2F]</p>
<p><span style="font-size: 150%;"><strong><span>2.2.3 -&nbsp;Capsense Joypad Driver</span></strong></span></p>
<p>The Capsense system of the Pioneer lets you configure and tune the capacitive areas on board.</p>
<p>For this project, I wanted to use the example provided by ModusToolbox as a starting point. This makes it more familiar to everyone. The example configures the horizontal slider as a linear potentiometer, while the two buttons as simple push-buttons. To turn it into a capacitive joypad, I changed the main function to read the slider as if it were a rotary encoder.&nbsp;</p>
<p>This means that variations of increasing values on the slider correspond to a movement to the right, while variations of decreasing values correspond to movements to the left. Having said that it's just a matter of coordinating the call to this refresh function at the time the screen is not being refreshed, ie during the vertical blank area.<br />What I think is important is to configure the controls well in the Capsense Tuner.</p>
<p><img style="display: block; height: 410px; margin-left: auto; margin-right: auto; max-height: 410px; max-width: 779px;" alt="" height="410" src="/resized-image/__size/1558x820/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/capsense_2D00_0.png" width="778" /></p>
<p>To obtain "clean" and smooth movements it is necessary to instruct the tuner by providing him with a profile on the background electromagnetic noise and on the signal level when the areas are touched with the fingers.</p>
<p><img style="display: block; height: 402px; margin-left: auto; margin-right: auto; max-height: 402px; max-width: 763px;" alt="" height="402" src="/resized-image/__size/1526x804/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/capsense_2D00_noise.png" width="762" /></p>
<p><img style="display: block; height: 401px; margin-left: auto; margin-right: auto; max-height: 401px; max-width: 760px;" alt="" height="376" src="/resized-image/__size/1520x802/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/capsense_2D00_signal.png" width="759" /></p>
<p>Once the profile has been created for all the capacitive areas, it is possible to test the controls, either through graphs or by viewing the levels. I am amazed by the Capsense system because it's unbelievable the amount of time and work that saves you.</p>
<p><img style="display: block; height: 401px; margin-left: auto; margin-right: auto; max-height: 401px; max-width: 759px;" alt="" height="368" src="/resized-image/__size/1518x802/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/capsense_2D00_signal_2D00_2.png" width="758" /></p>
<p><img style="display: block; height: 399px; margin-left: auto; margin-right: auto; max-height: 399px; max-width: 758px;" alt="" height="399" src="/resized-image/__size/1516x798/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/capsense_2D00_test_2D00_slider.png" width="757" /></p>
<p>I'll leave the details in the source code for further reading.</p>
<p><span style="font-size: 150%;"><strong>2.3 -&nbsp;Software side on CM4</strong></span></p>
<p>Since the CM4 core is faster, it takes care of running the entire emulator but lets the CM0+ core handle the synchronization. Explaining and detailing the hardware of the SEGA SG-1000 console would be long and cumbersome in this context, so if it is a topic of interest, I will continue the project later. The important point is how I made the two cores communicate.</p>
<p>In section 2.0 - Architecture, I showed that both cores have read and write access to specific memory locations. These locations function as memory-mapped registers. As I explained in this blog, I preferred to use this system because I found it to be faster than the IPC.<br />Since the CM0+ core needs to request the next scanline to be rendered quickly while a black line is displayed, reaction times are crucial.</p>
<p><span style="font-size: 150%;"><strong>2.3.1 -&nbsp;Memory mapped registers</strong></span></p>
<p>This system is very simple and relies on the use of SRAM which is shared by the cores. In practice, one core writes a value to one of these registers while the other core waits for that given register to assume a non-zero value. The calling core, after having written the value, eventually waits for it to return to zero, a sign that the second core has read it and therefore reset it.</p>
<p>This method is simple but care must be taken to handle pointers well. Better to create a common file for both cores, with the definition of an address map in the SRAM.</p>
<p>[embed:dc8ab71f-3b98-42d9-b0f6-e21e02a0f8e2:cfdd9dc8-b594-4ce7-9237-04774e45ad1e:type=c_cpp&text=%2F%2F%20Within%20the%20source%20file%20%2A.c%0Avolatile%20uint32_t%20gCommandAddr%20%20%20%20%20%20%3D%20%28SHARED_MEMORY%29%3B%0Avolatile%20uint32_t%20gCmdAddr%20%20%20%20%20%20%20%20%20%20%3D%20%28SHARED_MEMORY%20%2B%204%29%3B%0Avolatile%20uint32_t%20gVBlankAddr%20%20%20%20%20%20%20%3D%20%28SHARED_MEMORY%20%2B%208%29%3B%0Avolatile%20uint32_t%20gVideoBufferAddr%20%20%3D%20%28SHARED_MEMORY%20%2B%2012%29%3B%0Avolatile%20uint32_t%20gAudioBufferAddr%20%20%3D%20%28SHARED_MEMORY%20%2B%2016%29%3B%0Avolatile%20uint32_t%20gIOAddr%20%20%20%20%20%20%20%20%20%20%20%3D%20%28SHARED_MEMORY%20%2B%2020%29%3B%0A%0A...%0A%0A%2F%2F%20Within%20the%20header%20file%20%2A.h%0Aextern%20volatile%20uint32_t%20gCommandAddr%3B%0Aextern%20volatile%20uint32_t%20gCmdAddr%3B%0Aextern%20volatile%20uint32_t%20gVBlankAddr%3B%0Aextern%20volatile%20uint32_t%20gVideoBufferAddr%3B%0Aextern%20volatile%20uint32_t%20gAudioBufferAddr%3B%0Aextern%20volatile%20uint32_t%20gIOAddr%3B%0A%0A%2F%2F%20macros%0A%23define%20_SHARED_gCommand%20%20%2A%28%28uint8_t%2A%29%28%2A%28%28uint32_t%2A%29gCmdAddr%29%29%29%0A%23define%20_SHARED_gVBlank%20%20%20%2A%28%28uint8_t%2A%29%28%2A%28%28uint32_t%2A%29gVBlankAddr%29%29%29%0A%23define%20_SHARED_gIOPort%20%20%20%2A%28%28uint8_t%2A%29%28%2A%28%28uint32_t%2A%29gIOAddr%29%29%29%0A%0A...%0A%0A%2F%2F%20access%20methods%0A%2F%2F%20writing%3A%0A_SHARED_gCommand%20%3D%20n%3B%0A%0A%2F%2F%20reading%3A%0Avalue%20%3D%20_SHARED_gCommand%3B%0A%0A%23define%20_SHARED_gSemaPort%20%2A%28%28uint8_t%2A%29%28%2A%28%28uint32_t%2A%29gSemaAddr%29%29%29]</p>
<p>Later, where necessary, it will be easy to write to the registers and create a waiting cycle for them.&nbsp;In the following example, the core CM4 is waiting to receive a value from the core CM0+.</p>
<p>[embed:dc8ab71f-3b98-42d9-b0f6-e21e02a0f8e2:d4bd39f9-0243-41f1-b037-f87b37a889ef:type=c_cpp&text=%2F%2F%20cm0%2B%0A%20%20%20%20for%20%28%3B%3B%29%0A%20%20%20%20%7B%0A%20%20%20%20%20%20_SHARED_gCommand%20%3D%201%3B%0A%20%20%20%20%20%20while%20%28_SHARED_gCommand%29%20%7B%7D%3B%0A%20%20%20%20%7D]</p>
<p>[embed:dc8ab71f-3b98-42d9-b0f6-e21e02a0f8e2:ca694ca2-d230-4795-af42-5de047230ba2:type=c_cpp&text=%2F%2F%20cm4%0A%20%20%20%20for%20%28%3B%3B%29%0A%20%20%20%20%7B%0A%20%20%20%20%20%20while%20%28_SHARED_gCommand%20%3D%3D%200%29%20%7B%7D%3B%0A%20%20%20%20%20%20_SHARED_gCommand%20%3D%200%3B%0A%20%20%20%20%7D]</p>
<p><span style="font-size: 150%;"><strong>2.3.2 - Emulator Architecture</strong></span></p>
<p></p>
<p>A software computer emulator has to emulate all the components in a real computer using software programs over another computer or hardware. Basically, it's based on a virtual machine&nbsp;(VM) like Java and others. A virtual machine is any kind of computer that does not exist as real hardware but it is implemented by software. All hardware components that are in the hardware to emulate, actually are translated into code that will mimic as accurately as possible the behavior of the component.</p>
<p><span>In section 2.0 - Architecture, is shown the main schematic of the SEGA SG-1000 hardware.&nbsp;In my emulator, the various active components perform certain functions that can be called cyclically such as rendering graphics or video. In theory, to simulate hardware in real-time, all the components should execute a piece of program each in sequence, thus exploiting a memory-mapped register, at each scanline the CM0+ core calls the code parts in the CM4 core which executes program parts of the game.</span></p>
<p><span>Now, the problem that arises is respecting the times of the Video Driver, in fact, a video scanline lasts 31.778us, therefore the emulation corresponding to the time that the console takes to calculate everything necessary to draw a scanline, must last that time. This is very difficult, and a 150MHz CM4 core frequency is a bit low to do everything.</span></p>
<p><span>What I thought of doing is dividing the tasks into two different moments.</span></p>
<p><span> I will try to explain as best as possible and briefly how the SG-1000 works.</span></p>
<p><span>As soon as you turn on the console, the Z80 CPU starts reading the code from the ROM contained in the cartridge. Running game code will ask the Z80 to read and write data to RAM, prepare graphics by accessing VDP registers, or change frequency to PSG audio oscillators.</span></p>
<p><span>The VDP and PSG also have continuous output which is affected by internal registers which are modified by the Z80. Usually to obtain some stability the Z80 accesses these components when the VDP is not drawing the video frame. Then, to reduce the number of operations that the CM4 core has to perform in the time of a scanline, I can render only the graphics during the Active Area of the VGA signal, and subsequently during the vertical blank area, run the Z80 and render the samples audio.</span></p>
<p><span style="font-size: 150%;"><strong>3.0 - Conclusion</strong></span></p>
<p>Summing up, this project allowed me to get to know the PSoC62, and at the same time share a learning path with all of you, developing an application that could push this MCU to the maximum.</p>
<p>An emulator requires a lot of resources, and so I set myself this challenge, but what are the limitations I've encountered?<br />First the 128Kb memory for a project that uses graphics and sound is limited, especially if you need a video frame buffer to render to. Suffice it to say that if we want to implement an RGB video driver we have to spend 76800 bytes for a frame buffer with a resolution of 320x240. This emulator has been heavily modified to run "comfortably" with this amount of SRAM.</p>
<p>However this emulator could support all titles released for SEGA SG-1000, but obviously, in the short time available it was tested only for the proposed game "Space Invaders". On the other hand, a heavy limitation for the choice of the game is given by the presence of only one axis for the capacitive joypad.</p>
<p>As far as speed is concerned, you have to pay close attention to how to configure the clocks, bearing in mind that both cores share the same clock source and the frequency of the two cores must be balanced with dividers, therefore the maximum obtainable is 150Mhz on CM4 and 75MHz on CM0+. If, on the other hand, you want to reach 100MHz on CM0+, the CM4 core also drops to 100MHz.</p>
<p>Another aspect to consider is the use of memory which is shared, and for correct use and based on the application to be developed, it may require the configuration of the protection units, as well as the synchronization of accesses by the two cores.</p>
<p>Otherwise, the tools offered by ModusToolbox are extremely useful and easy to use. I am very happy with this experience, and I hope I have left something for everyone by turning the Pioneer Kit into a mini-game console.</p>
<p><span style="font-size: 150%;"><strong>3.1 -&nbsp;The project</strong></span></p>
<p>This project is available on GitHub so that&nbsp;you all&nbsp;can contribute to it later on if you like it.</p>
<p></p>
<p>Anyway,&nbsp;I am attaching the compiled *.hex file below which can be loaded into the FLASH of the board for ready use.</p>
<p>[View:~/cfs-file/__key/communityserver-components-multipleuploadfilemanager/847abbf0_2D00_8975_2D00_4c55_2D00_a460_2D00_ff5387c359f6-245556-complete/ATCDC_5F00_PSoC62_5F00_SG_2D00_1000.zip:128:206]</p>