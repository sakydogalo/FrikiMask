# FrikiMask

## Hardware

### Fritzing

## Software

### Arduino

### Editor

## Intructions set of serial port
Commands:

	r: "r123"   = Set Red 123       [0-255] Don't write in eeprom
	g: "g123"   = Set Green 123     [0-255] Don't write in eeprom
	b: "b123"   = Set Blue 123      [0-255] Don't write in eeprom
	i: "i10"    = Set brightness 10 [0-255] Don't write in eeprom

	d: "d1"     = Display smile of the current mask [0-4] mantained 2 seconds
	p: "p"      = Play 5 times

	c: "c"      = view info current mask

	f: "f0"		= Load mask from flash  i[0-99]
	e: "e0"		= Load mask from EEprom i[0-9]
	s: "s2"		= Save mask to eeprom i[0-9]
	k: "k0" 	= Reset eeprom mask from flash to eeprom [0-4]
	w: "w"		= Reset all eeprom

	D: "D"		= Dinamic smille
	S: "S"		= Static smille


## Access to Eeprom

	1 mask contain:
	5 mouths
		1 mouth contain:
			8 lines = 8 bytes
				1 line = 8 leds = 8 bits = 1byte
	5 x 8 bytes
	2bytes for brightness
	3bytes for color (RGB)

	Complete mask ocupes 45bytes

	mask 0 == position 0  --> first mouth
		  position 8  --> second mouth
		  position 16 --> thirt mouth
		  position 24 --> fourth
		  position 32 --> fifth
		  position 40 --> brightness
		  position 42 --> Colour (RGB)
		     position 42 --> Red
		     position 43 --> Green
		     position 44 --> Blue
	mask 1 == position 45
	mask 2 == position 90
	.
	.
	mask 9 == position 405

## References

	https://github.com/chrissicool/zevoicemask
	https://github.com/danijerez/voicemask
	https://www.kickstarter.com/projects/jabbermask/jabbermask
