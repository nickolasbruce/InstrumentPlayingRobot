riot.tag2('instrument', '<div class="inst_container {opts.name}"> <div class="header"> <span>{opts.name}</span> <form> USB PORT:<br> <input class="port {opts.name}" type="text" name="port"> </form> </div> <div class="white_key c {opts.name}"></div> <div class="black_key cs {opts.name}"></div> <div class="white_key d {opts.name}"></div> <div class="black_key ds {opts.name}"></div> <div class="white_key e {opts.name}"></div> <div class="white_key f {opts.name}"></div> <div class="black_key fs {opts.name}"></div> <div class="white_key g {opts.name}"></div> <div class="black_key gs {opts.name}"></div> <div class="white_key a {opts.name}"></div> <div class="black_key as {opts.name}"></div> <div class="white_key b {opts.name}"></div> <div class="white_key chigh {opts.name}"></div> <div class="invis {opts.name}"></div> </div>', 'instrument .inst_container,[data-is="instrument"] .inst_container{ position: relative; margin: 10px; width: 53vw; height: 38vw; border-radius: 1vw; border: 3px solid; background-color: white; } instrument .header,[data-is="instrument"] .header{ position: absolute; width: 49vw; height: 8.7vw; left: 2vw; top: 2vw; background-size: contain; background-image: url(\'header.png\'); } instrument .header span,[data-is="instrument"] .header span{ position: absolute; font-family: sans-serif; color: #10AAAF; font-size: 6vh; font-weight: bold; text-shadow: -1px -1px 0 #000, 1px -1px 0 #000, -1px 1px 0 #000, 1px 1px 0 #000; top: 4vw; left: 2vw; } instrument .header form,[data-is="instrument"] .header form{ position: absolute; top: 3.8vw; left: 30vw; color: #10AAAF; font-family: sans-serif; text-shadow: -1px -1px 0 #000, 1px -1px 0 #000, -1px 1px 0 #000, 1px 1px 0 #000; } instrument .header form input,[data-is="instrument"] .header form input{ background-color: #333; border-left: 2px solid #10AAAF; color: #29a329; } instrument .white_key,[data-is="instrument"] .white_key,instrument .black_key,[data-is="instrument"] .black_key{ position: absolute; top: 10.5vw; background-size: cover; } instrument .white_key,[data-is="instrument"] .white_key{ width: 6vw; height: 25.8vw; background-image: url(\'white_sprite.png\'); background-position: 0 0; z-index: 100; } instrument .white_key.active,[data-is="instrument"] .white_key.active{ background-position: 0 25.8vw; } instrument .black_key,[data-is="instrument"] .black_key{ width: 3.8vw; height: 14.4vw; background-image: url(\'black_sprite.png\'); background-position: 0 0; z-index: 200; } instrument .black_key.active,[data-is="instrument"] .black_key.active{ background-position: 0 14.4vw; } instrument .c,[data-is="instrument"] .c{ left: 2.5vw; } instrument .cs,[data-is="instrument"] .cs{ left: 6.57vw; } instrument .d,[data-is="instrument"] .d{ left: 8.5vw; } instrument .ds,[data-is="instrument"] .ds{ left: 12.57vw; } instrument .e,[data-is="instrument"] .e{ left: 14.5vw; } instrument .f,[data-is="instrument"] .f{ left: 20.5vw; } instrument .fs,[data-is="instrument"] .fs{ left: 24.57vw; } instrument .g,[data-is="instrument"] .g{ left: 26.5vw; } instrument .gs,[data-is="instrument"] .gs{ left: 30.57vw; } instrument .a,[data-is="instrument"] .a{ left: 32.5vw; } instrument .as,[data-is="instrument"] .as{ left: 36.57vw; } instrument .b,[data-is="instrument"] .b{ left: 38.5vw; } instrument .chigh,[data-is="instrument"] .chigh{ left: 44.5vw; } instrument .invis,[data-is="instrument"] .invis{ visibility: none; }', '', function(opts) {

		function getCookie(name) {
	    var cookieValue = null;
	    if (document.cookie && document.cookie !== '') {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
          var cookie = jQuery.trim(cookies[i]);

          if (cookie.substring(0, name.length + 1) === (name + '=')) {
            cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
            break;
          }
        }
	    }
	    return cookieValue;
		}

		var clickedKey = function(e) {

			e.classList.add('active');
			setTimeout(function() {
				e.classList.remove('active');
			}, 250);

			classes = e.className.split(" ");
			port_input = document.getElementsByClassName("port " + classes[2])[0];
			usb_port = port_input.value.replace(/^\s+|\s+$/g, '');
			payload =
				JSON.stringify({
					name: classes[2],
					key: classes[1],
					port: usb_port
				});

			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function() {
				if (this.readyState == 4 && this.status == 200) {
					container = document.getElementsByClassName("inst_container " + classes[2])[0];
					container.setAttribute("style", "background-color: #e6ffe6;");
					setTimeout(function() {
						container.setAttribute("style", "background-color: #ffffff;");
					}, 100);
				}
			};
			xhttp.open("POST", "/json", true);
			http.setRequestHeader("X-CSRFToken", getCookie("csrftoken"));
			http.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
			xhttp.send(payload);
		}

		this.on('mount', function() {

			white = document.getElementsByClassName(opts.name + " white_key");
			for (var i=0; i<white.length; i++) {
				white[i].addEventListener('click', function(n) {
					return function() {clickedKey(n)};
				}(white[i]), false);
			}

			black = document.getElementsByClassName(opts.name + " black_key");
			for (var i=0; i<black.length; i++) {
				black[i].addEventListener('click', function(n) {
					return function() {clickedKey(n)};
				}(black[i]), false);
			}
		})

		document.onkeydown = function(event){
			var key = event.which || event.keyCode;
			for (var prop in opts.opts) {
				for(var index in opts.opts[prop]) {
					if (opts.opts[prop][index]==String.fromCharCode(key+32)) {
						classes = prop + " " + index;
						e = document.getElementsByClassName(classes)[0];
						clickedKey(e);
					}
				}
			}
		}

});