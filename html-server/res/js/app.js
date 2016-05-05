
(function() {
    $(document).foundation();
	// admin interface toggle
	if (window.location.hash == "#admin") {
		$("#admin").show();
		hookAdmin();
		return;
	}
    $("#tabs").show();
	hookTabs();

	function dataToObject(data) {
		if (data == "ERROR")
			return {};
		var obj = {};

		obj["time"] = data[0];
		obj["teams"] = data[1];
		obj["problems"] = data[2];

		obj["team_data"] = [];
		obj["problem_data"] = [];

		var counter = 2;

		var i, j, teamNumber = 0;
		for (i = obj["teams"] - 1; i >= 0; i--) {
			var team = {
                "number": (++teamNumber),
				"name": data[++counter],
				"points": parseInt(data[++counter]),
				"correct": parseInt(data[++counter]),
				"bonus": parseInt(data[++counter]),
				"answers": [],
			};
			for (j = obj["problems"] - 1; j >= 0; j--) {
				team["answers"].push(parseInt(data[++counter]));
			}
			obj["team_data"].push(team);
		}

		for (i = obj["problems"] - 1; i >= 0; i--) {
			var problem = {
				"points": parseInt(data[++counter]),
				"answered": parseInt(data[++counter]),
				"bonus": parseInt(data[++counter]),
				"answerers": [],
			};
			for (j = obj["teams"] - 1; j >= 0; j--) {
				problem["answerers"].push(parseInt(data[++counter]));
			}
			obj["problem_data"].push(problem);
		}

		return obj;
	}

	function hookAdmin() {
		var $teamPopup = $("#teamPopup");
		var $teamSetPopup = $("#teamSetPopup");
		var $teamAddPopup = $("#teamAddPopup");
		var obj = null;
		function appendClosePopup($element, interiorHTML) {
			$element.append("<div class='theclose'>" + interiorHTML + "</div>");
			$element.find('.theclose').click(function () {
				$element.toggleClass('visible');
			});
		}
		function handleClickPopup() {
			function move(to) {
				to(index);
				setTimeout(function () {
					$teamPopup.toggleClass('visible');
				}, 250);
			}
			var index = this.getAttribute('data-index');
			$teamPopup.toggleClass('visible');
			$teamPopup.html("<div class='twobutton left' style='left:0'><span><span>ADD<br/>PROBLEM</span></span></div>");
			$teamPopup.append("<div class='twobutton right' style='right:0'><span><span>SET<br/>JOLLY</span></span></div>");
			appendClosePopup($teamPopup, "<p><strong>" + obj["team_data"][index].name + "</strong></p><p>CLOSE</p>");
			console.log('handling click', index);

			$teamPopup.find('.twobutton.left').click(move.bind(this, handleClickAddPopup));
			$teamPopup.find('.twobutton.right').click(move.bind(this, handleClickSetPopup));
		}
		function handleClickAddPopup(index) {
			$teamAddPopup.toggleClass('visible');
			$teamAddPopup.html("");
			appendClosePopup($teamAddPopup, "<p>ADD PROBLEM</p><p><strong>" + obj["team_data"][index].name + "</strong></p><p>CLOSE</p>");
			var i;
			var text = "<div style='top:0;' class='didi'><div class='small-up-1 medium-up-5 row text-center' data-equalizer data-equalize-on='medium' data-equalize-by-row='true' style='margin-top: 1rem;'>";
			for (i = 0; i < obj["problems"] / 2; i++) {
				text += "<div class='column'><button class='button large expanded' data-equalizer-watch data-index='" + i + "'>problema " + (i + 1) + "</button></div>";
			}
			text += "</div></div>";
			text += "<div style='bottom:0;' class='didi'><div class='small-up-1 medium-up-5 row text-center' data-equalizer data-equalize-on='medium' data-equalize-by-row='true'>";
			for (i = obj["problems"] / 2; i < obj["problems"]; i++) {
				text += "<div class='column'><button class='button large expanded' data-equalizer-watch data-index='" + i + "'>problema " + (i + 1) + "</button></div>";
			}
			text += "</div></div>";
			$teamAddPopup.append(text);
			$teamAddPopup.append("<div style='top: 50%;transform: translateY(-50%);' class='didi'><input type='text' maxlength='4'/></div>");
			var r = $teamAddPopup.find('input[type=text]')[0];
			$teamAddPopup.find("button[data-index]").click(function (e) {
				var indexAnswer = parseInt(r.value).toString();
				if (isNaN(parseInt(r.value)))
					indexAnswer = "0";
				while (indexAnswer.length != 4)
					indexAnswer = "0" + indexAnswer;
				var indexProblem = 1 + +this.getAttribute('data-index') ;
				var indexTeam = +index + 1;
				e.preventDefault();
				r.focus();
				if (confirm(indexAnswer + ' for ' + obj["team_data"][index].name.trim()  + ' (' + (index + 1) + ') to ' + indexProblem)) {
					$.get('/a_' + indexTeam + "_" + indexProblem + "_" + indexAnswer + "_", function (data) {
						console.log(data);
						$teamAddPopup.toggleClass('visible');
					});

				}
				return false;
			});

			console.log(r);
			setTimeout(function () {
				r.focus();
			}, 25);
		}
		function handleClickSetPopup(index) {
			$teamSetPopup.toggleClass('visible');
			$teamSetPopup.html("");
			appendClosePopup($teamSetPopup, "<p>SET JOLLY</p><p><strong>" + obj["team_data"][index].name + "</strong></p><p>CLOSE</p>");
			var i;
			var text = "<div style='top:0;' class='didi'><div class='small-up-1 medium-up-5 row text-center' data-equalizer data-equalize-on='medium' data-equalize-by-row='true' style='margin-top: 1rem;'>";
			for (i = 0; i < obj["problems"] / 2; i++) {
				text += "<div class='column'><button class='button large expanded' data-equalizer-watch data-index='" + i + "'>problema " + (i + 1) + "</button></div>";
			}
			text += "</div></div>";
			text += "<div style='bottom:0;' class='didi'><div class='small-up-1 medium-up-5 row text-center' data-equalizer data-equalize-on='medium' data-equalize-by-row='true'>";
			for (i = obj["problems"] / 2; i < obj["problems"]; i++) {
				text += "<div class='column'><button class='button large expanded' data-equalizer-watch data-index='" + i + "'>problema " + (i + 1) + "</button></div>";
			}
			text += "</div></div>";
			$teamSetPopup.append(text);
			$teamSetPopup.find("button[data-index]").click(function (e) {
				var indexProblem = 1 + +this.getAttribute('data-index') ;
				var indexTeam = +index + 1;
				e.preventDefault();
				if (confirm('JOLLY for ' + obj["team_data"][index].name.trim()  + ' (' + (index + 1) + ') to ' + indexProblem)) {
					$.get('/S_' + indexTeam + "_" + indexProblem, function (data) {
						console.log(data);
						$teamSetPopup.toggleClass('visible');
					});

				}
				return false;
			});
		}
		$.get('/data', function (data) {
            if (data == "ERROR") {
                setTimeout(function () {
                    hookAdmin();
                }, 2000);
                return;
            }
			obj = dataToObject(data.split('|'));
			var $teamSelector = $("#teamSelector");
			var i;
			console.log(obj["team_data"]);
			var text = "<div class='small-up-1 medium-up-5 row text-center' data-equalizer data-equalize-on='medium' data-equalize-by-row='true'>";
			for (i = 0; i <= obj["teams"] - 1; i++) {
				text += "<div class='column'><div class='button large secondary expanded' data-equalizer-watch data-index='" + i + "'><p>" + (i + 1) + "</p>" + obj["team_data"][i].name + "</div></div class='column'>";
			}

			text += "</div>";
			$teamSelector.html(text);
			$teamSelector.foundation();
			$teamSelector.find(".button").click(handleClickPopup);
		});


	}

	function parseTime(timeLeft) {
		if (timeLeft == undefined)
			return "BAFTA";
		if (timeLeft < 0)
			return "FELICITARI";
		var hours = parseInt(timeLeft / 3600);
		var minutes = parseInt((timeLeft % 3600) / 60);
		var seconds = parseInt(timeLeft % 60);

		var text = "";
		if (hours) {
			if (hours < 10)
				text += "0";
			text += hours + ":";
		}
		if (minutes < 10)
			text += "0";
		text += minutes + ":";
		if (seconds < 10)
			text += "0";
		text += seconds;
		return text;
	}

	function hookTabs() {
		var $timeleft = $("#timeleft");
		var $clasament = $("#clasament");
		var $probleme = $("#probleme");
		var $tabel = $("#tabel");
		var displayed = false;
		function updateClasament(obj) {
			obj["team_data"].sort(function (a,b) {
				return b.points - a.points;
			});
			var max = -1;
			var i;
			for (i = 0; i < obj["teams"]; i++) {
				var punctaj = obj["team_data"][i].points;
				max = max > punctaj ? max : punctaj;
			}
			for (i = 0; i < obj["teams"]; i++) {
				$("#teamname_" + i).html(obj["team_data"][i]["number"] + '. <strong>' + obj["team_data"][i]["name"] + '</strong>');
				var theClass = "secondary";
				if (max == obj["team_data"][i].points)
					theClass = "success";

				$("#teamprocent_" + i).css("width", parseInt(obj["team_data"][i].points / max * 100) + "%").parent().removeClass("success secondary").addClass(theClass);
				$("#teamscore_" + i).html("<strong>" + obj["team_data"][i].points + "</strong>");
			}
		}
		function updateProbleme(obj) {
			var max = -1;
			var i;
			for (i = 0; i < obj["problems"]; i++) {
				var punctaj = obj["problem_data"][i].points;
				max = max > punctaj ? max : punctaj;
			}

			for (i = 0; i < obj["problems"]; i++) {
				var theClass = "secondary";
				if (max == obj["problem_data"][i].points)
					theClass = "success";

				$("#problemprocent_" + i).css("width", parseInt(obj["problem_data"][i].points / max * 100) + "%").parent().removeClass("success secondary").addClass(theClass);
				$("#problemscore_" + i).html("<strong>" + obj["problem_data"][i].points + "</strong>");
			}
		}
		function updateTabel(obj) {
			var ths = "";
			for (i = 1; i <= obj["problems"]; i++) {
				ths += "<th>" + i + "</th>";
			}
			var table = "<table><thead><tr><th width='300'>Nume Echipa</th>" + ths +"</tr></thead><tbody>";
			for (i = 0; i < obj["teams"]; i++) {
				table += "<tr><th class='text-right'><small>" + obj["team_data"][i].number + ".</small> " + obj["team_data"][i].name + "</th>";

				for (j = 0; j < obj["problems"]; j++) {
					var classes = "alert";
					if (obj["problem_data"][j].answerers[i])
						classes = "success";

					if (obj["team_data"][i].answers[j] == 0)
						classes = "secondary";

					if (obj["team_data"][i].bonus - 1 == j)
						classes += " bordered";

					var span = "<span class='label " + classes + "'>" + obj["team_data"][i].answers[j] + "</span>";

					table += "<td>" + span + "</td>";
				}
				table += "</tr>";
			}
			table += "</tbody></table>";
			$tabel.html(table);
		}

		var updateTimeLeftTicks = 0;
		function updateTimeLeft(left, string) {
			updateTimeLeftTicks++;
			if (updateTimeLeftTicks >= 4) {
				updateTimeLeftTicks = 0;
				if (left < 6400) {
					if (left > 1800) {
						$timeleft.toggleClass("secondary");
					} else {
						$timeleft.toggleClass("alert");
					}
				}
			}
			$timeleft.html(string);
		}
		function updateData(data) {
			obj = dataToObject(data);
			updateTimeLeft(obj["time"], parseTime(obj["time"]));


			if (!displayed) {
				$timeleft.css("display", "table");
			}

			if (data == "ERROR")
				return;

			if (!displayed) {
				updateDisplay(obj);
				setTimeout(function () {
					updateData(data);
				}, 100)
				return;
			}

			updateTabel(obj);
			updateClasament(obj);
			updateProbleme(obj);
		}
		function updateDisplay(obj) {
			console.log(obj);
			var i, j;
			for (i = 0; i < obj["teams"]; i++) {
				$clasament.append(
					'<div class="row">' +
					'<div class="small-3 columns text-right" id="teamname_' + i + '"><strong>' + obj["team_data"][i]["name"] + '</strong></div>' +
					'<div class="small-8 columns">' +
					'<div class="success progress" role="progressbar">' +
					  '<div class="progress-meter" style="width: 0%" id="teamprocent_' + i + '"></div>' +
					'</div></div><div class="small-1 columns" id="teamscore_' + i + '"></div></div>');
			}
			for (i = 0; i < obj["problems"]; i++) {
				$probleme.append(
					'<div class="row">' +
					'<div class="small-3 columns text-right"><strong>problema ' + (i + 1) + '</strong></div>' +
					'<div class="small-8 columns">' +
					'<div class="success progress" role="progressbar">' +
					  '<div class="progress-meter" style="width: 0%" id="problemprocent_' + i + '"></div>' +
					'</div></div><div class="small-1 columns" id="problemscore_' + i + '"></div></div>');
			}
			var table = "<table><thead><tr><th width='300'>Nume Echipa</th><th colspan='" + obj["problems"] + "'>Problema</th></tr></thead><tbody>";
			for (i = 0; i < obj["teams"]; i++) {
				table += "<tr><th class='text-right'>" + (i + 1) + ". " + obj["team_data"][i].name + "</th>";

				for (j = 0; j < obj["problems"]; j++) {
					var classes = "alert";
					if (obj["problem_data"][j].answerers[i])
						classes = "success";

					if (obj["team_data"][i].bonus - 1 == j)
						classes += " bordered";

					var span = "<span class='label " + classes + "'>" + obj["team_data"][i].answers[j] + "</span>";

					table += "<td>" + span + "</td>";
				}
				table += "</tr>";
			}
			table += "</tbody></table>";
			$tabel.html(table);
			displayed = true;
		}
		var oldTime = Date.now() - 1000;
		function updateInterval() {
			if (parseInt(Date.now() / 1000) != oldTime) {
				oldTime = parseInt(Date.now() / 1000);
				$.get('/data', function (data) {
					updateData(data.split('|'));

				});
			}
			requestAnimationFrame(updateInterval);
		}
		updateInterval();
	}

})();
