start-microcode randreg

: main

	d# 0
    
	begin

		RANDOM c@ over c!

		h# 280E
	again

;



end-microcode
