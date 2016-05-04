(function() {
	$(document).foundation()
	$(function() {

		// admin interface toggle
		if (window.location.hash == "#admin") {
			$("#tabs").hide();
			$("#admin").show();
			hookAdmin();
			return;
		}

		hookTabs();
	});


	function hookAdmin() {

	}

	function hookTabs() {
		var $timeleft = $("#timeleft");
		var displayed = false;
		function updateData(data) {
			$timeleft.html(parseInt(data[0] / 60) + " min " + parseInt(data[0] % 60) + " sec");
			console.log(data);
		}
		function updateDisplay() {
			$timeleft.show();
			
			displayed = true;
		}
		function updateInterval() {
			if (!displayed)
				updateDisplay();
			
			$.get('/data', function (data) {
				updateData(data.split('|'));
				setTimeout(updateInterval, 1000);
			});
		}
		updateInterval();
	}

})();