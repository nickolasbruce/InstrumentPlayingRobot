riot.tag2('instrument', '<div class="inst_container {opts.name}"> <div class="header"> <span>{opts.name}</span> <form> PORT:<br> <input class="port {opts.name}" type="text" name="port"> </form> </div> <div class="white_key c {opts.name}"></div> <div class="black_key cs {opts.name}"></div> <div class="white_key d {opts.name}"></div> <div class="black_key ds {opts.name}"></div> <div class="white_key e {opts.name}"></div> <div class="white_key f {opts.name}"></div> <div class="black_key fs {opts.name}"></div> <div class="white_key g {opts.name}"></div> <div class="black_key gs {opts.name}"></div> <div class="white_key a {opts.name}"></div> <div class="black_key as {opts.name}"></div> <div class="white_key b {opts.name}"></div> <div class="white_key ch {opts.name}"></div> </div> <div class="counter {opts.name}"></div>', 'instrument .inst_container,[data-is="instrument"] .inst_container{ position: relative; margin: 10px; width: 44vw; height: 32vw; border-radius: 1vw; border: 3px solid; background-color: white; } instrument .header,[data-is="instrument"] .header{ position: absolute; width: 40.8vw; height: 7.2vw; left: 2.1vw; top: 2vw; background-size: contain; background-image: url(\'/static/instruments/header.png\'); } instrument .header span,[data-is="instrument"] .header span{ position: absolute; font-family: sans-serif; color: #10AAAF; font-size: 5vh; font-weight: bold; text-shadow: -1px -1px 0 #000, 1px -1px 0 #000, -1px 1px 0 #000, 1px 1px 0 #000; top: 3.6vw; left: 2vw; } instrument .header form,[data-is="instrument"] .header form{ position: absolute; top: 3vw; left: 24vw; color: #10AAAF; font-family: sans-serif; text-shadow: -1px -1px 0 #000, 1px -1px 0 #000, -1px 1px 0 #000, 1px 1px 0 #000; } instrument .header form input,[data-is="instrument"] .header form input{ background-color: #333; border-left: 2px solid #10AAAF; color: #29a329; } instrument .white_key,[data-is="instrument"] .white_key,instrument .black_key,[data-is="instrument"] .black_key{ position: absolute; top: 9.2vw; background-size: cover; } instrument .white_key,[data-is="instrument"] .white_key{ width: 5vw; height: 21.4vw; background-image: url(\'/static/instruments/white_sprite.png\'); background-position: 0 0; z-index: 100; } instrument .white_key.active,[data-is="instrument"] .white_key.active{ background-position: 0 21.4vw; } instrument .black_key,[data-is="instrument"] .black_key{ width: 3.2vw; height: 12vw; background-image: url(\'/static/instruments/black_sprite.png\'); background-position: 0 0; z-index: 200; } instrument .black_key.active,[data-is="instrument"] .black_key.active{ background-position: 0 12vw; } instrument .c,[data-is="instrument"] .c{ left: 2.5vw; } instrument .cs,[data-is="instrument"] .cs{ left: 5.9vw; } instrument .d,[data-is="instrument"] .d{ left: 7.5vw; } instrument .ds,[data-is="instrument"] .ds{ left: 10.9vw; } instrument .e,[data-is="instrument"] .e{ left: 12.5vw; } instrument .f,[data-is="instrument"] .f{ left: 17.5vw; } instrument .fs,[data-is="instrument"] .fs{ left: 20.9vw; } instrument .g,[data-is="instrument"] .g{ left: 22.5vw; } instrument .gs,[data-is="instrument"] .gs{ left: 25.9vw; } instrument .a,[data-is="instrument"] .a{ left: 27.5vw; } instrument .as,[data-is="instrument"] .as{ left: 30.9vw; } instrument .b,[data-is="instrument"] .b{ left: 32.5vw; } instrument .ch,[data-is="instrument"] .ch{ left: 37.5vw; } instrument .counter,[data-is="instrument"] .counter{ visibility: hidden; }', '', function(opts) {

		var clickedKey = function(e) {

			e.classList.add('active');

			document.addEventListener('keyup', function() {
				e.classList.remove('active');
			});

			document.addEventListener('mouseup', function() {
				e.classList.remove('active');
			});

			classes = e.className.split(" ");
			port_input = document.getElementsByClassName("port " + classes[2])[0];
			port_input = port_input.value.replace(/^\s+|\s+$/g, '');

			counter = document.getElementsByClassName("counter " + classes[2])[0];
			count = parseInt(counter.innerHTML) + 1;
			counter.innerHTML = count;

			payload =
				JSON.stringify({
					name: classes[2],
					key: classes[1],
					port: port_input,
					count: counter.innerHTML
				});

			fetch('/play/jsonhandler/', {
		    method: 'POST',
		    body: payload,
		    headers: {
		    	"X-CSRFToken": "CSRF_COOKIE",
	        "Content-Type": "application/x-www-form-urlencoded; charset=utf-8"
		    },
		    credentials: 'include',
		    mode: 'cors',
		    cache: 'default',
			});
		}

		this.on('mount', function() {

			counter = document.getElementsByClassName(opts.name + " counter")[0];
			counter.innerHTML = 0;

			white = document.getElementsByClassName(opts.name + " white_key");
			for (var i=0; i<white.length; i++) {
				white[i].addEventListener('mousedown', function(n) {
					return function() {clickedKey(n)};
				}(white[i]), false);
			}

			black = document.getElementsByClassName(opts.name + " black_key");
			for (var i=0; i<black.length; i++) {
				black[i].addEventListener('mousedown', function(n) {
					return function() {clickedKey(n)};
				}(black[i]), false);
			}

		})

		document.onkeydown = function(event){
			var key = event.which || event.keyCode;
			for (var num in opts.opts) {
				for(var note in opts.opts[num]) {
					instrument = opts.opts[num]
					if (instrument[note]==String.fromCharCode(key+32)) {
						classes = instrument["name"] + " " + note;
						e = document.getElementsByClassName(classes)[0];
						clickedKey(e);
					}
				}
			}
		}

});