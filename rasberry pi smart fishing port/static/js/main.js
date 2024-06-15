document.addEventListener("DOMContentLoaded", function() {
  const temp = document.getElementById('temp');
  const waterL = document.getElementById('waterL');
  const turb = document.getElementById('turb');
  const ph = document.getElementById('ph');

  const notification = document.createElement('div');
  notification.className = 'alert alert-warning';
  notification.style.display = 'none';
  document.body.prepend(notification);

  let warnings = [];

  temperature = new ProgressBar.SemiCircle(temp, {
    strokeWidth: 6,
    color: "#FFEA82",
    trailColor: "#eee",
    trailWidth: 1,
    easing: "easeInOut",
    duration: 1400,
    svgStyle: null,
    text: {
      value: "",
      alignToBottom: false,
    },
    from: { color: "#FFEA82" },
    to: { color: "#ED6A5A" },
    step: (state, bar) => {
      let value = bar.value();
      value = value * 50 - 10; // -10 ~ 40 범위로 변환
      bar.setText(value ? `${Math.round(value)}°C` : "");
      bar.text.style.color = state.color;

      // 온도에 따른 색상 변경 및 알림
      if (value < 25) {
        bar.path.setAttribute("stroke", "#000080"); // 남색
        addWarning("너무 차갑습니다!");
      } else if (value < 26) {
        bar.path.setAttribute("stroke", "#0000FF"); // 파랑
        removeWarning("너무 차갑습니다!");
      } else if (value < 27) {
        bar.path.setAttribute("stroke", "#00FF00"); // 초록
        removeWarning("너무 차갑습니다!");
      } else if (value < 28) {
        bar.path.setAttribute("stroke", "#FFFF00"); // 노랑
        removeWarning("너무 차갑습니다!");
      } else if (value < 29) {
        bar.path.setAttribute("stroke", "#FF0000"); // 빨강
        removeWarning("너무 차갑습니다!");
      } else {
        bar.path.setAttribute("stroke", "#FF0000"); // 빨강
        addWarning("너무 뜨겁습니다!");
      }
    },
  });
  temperature.text.style.fontSize = "2rem";

  water_level = new ProgressBar.SemiCircle(waterL, {
    strokeWidth: 6,
    color: "#FFEA82",
    trailColor: "#eee",
    trailWidth: 1,
    easing: "easeInOut",
    duration: 1400,
    svgStyle: null,
    text: {
      value: "",
      alignToBottom: false,
    },
    from: { color: "#FFEA82" },
    to: { color: "#ED6A5A" },
    step: (state, bar) => {
      let value = Math.round(bar.value() * 1000); // 0 ~ 1000 범위로 변환
      bar.setText(value ? `${value} mm` : "");
      bar.text.style.color = state.color;

      // 수위에 따른 색상 변경 및 알림
      if (value < 100) {
        bar.path.setAttribute("stroke", "#FF0000"); // 빨강
        addWarning("물이 없습니다!");
      } else {
        bar.path.setAttribute("stroke", `rgb(${255 - (value / 1000) * 255}, ${value / 1000 * 255}, 0)`); // 수위에 따라 색상 변경
        removeWarning("물이 없습니다!");
      }
    },
  });
  water_level.text.style.fontSize = "2rem";

  turbidity = new ProgressBar.SemiCircle(turb, {
    strokeWidth: 6,
    color: "#FFEA82",
    trailColor: "#eee",
    trailWidth: 1,
    easing: "easeInOut",
    duration: 1400,
    svgStyle: null,
    text: {
      value: "",
      alignToBottom: false,
    },
    from: { color: "#FFEA82" },
    to: { color: "#ED6A5A" },
    step: (state, bar) => {
      let value = Math.round(bar.value() * 1000); // 0 ~ 1000 범위로 변환
      bar.setText(value ? `${value} NTU` : "");
      bar.text.style.color = state.color;

      // 탁도에 따른 알림
      if (value < 500) {
        bar.path.setAttribute("stroke", "#FF0000"); // 빨강
        addWarning("물이 탁합니다!");
      } else {
        bar.path.setAttribute("stroke", "#00FF00"); // 초록
        removeWarning("물이 탁합니다!");
      }
    },
  });
  turbidity.text.style.fontSize = "2rem";

  poh = new ProgressBar.SemiCircle(ph, {
    strokeWidth: 6,
    color: "#FFEA82",
    trailColor: "#eee",
    trailWidth: 1,
    easing: "easeInOut",
    duration: 1400,
    svgStyle: null,
    text: {
      value: "",
      alignToBottom: false,
    },
    from: { color: "#FFEA82" },
    to: { color: "#ED6A5A" },
    step: (state, bar) => {
      let value = bar.value() * 13; // 0 ~ 13 범위로 변환
      bar.setText(value ? `${Math.round(value * 10) / 10} pH` : ""); // 소수점 한자리까지 표시
      bar.text.style.color = state.color;

      // pH에 따른 알림
      if (value > 9) {
        bar.path.setAttribute("stroke", "#FF0000"); // 빨강
        addWarning("pH가 높습니다!");
      } else if (value < 6) {
        bar.path.setAttribute("stroke", "#0000FF"); // 파랑
        addWarning("pH가 낮습니다!");
      } else {
        bar.path.setAttribute("stroke", "#00FF00"); // 초록
        removeWarning("pH가 높습니다!");
        removeWarning("pH가 낮습니다!");
      }
    },
  });
  poh.text.style.fontSize = "2rem";

  setInterval(fetchSensorData, 1000); // 주기적으로 1초마다 데이터를 가져옴

  function addWarning(message) {
    if (!warnings.includes(message)) {
      warnings.push(message);
      updateNotification();
    }
  }

  function removeWarning(message) {
    const index = warnings.indexOf(message);
    if (index !== -1) {
      warnings.splice(index, 1);
      updateNotification();
    }
  }

  function updateNotification() {
    if (warnings.length > 0) {
      notification.innerText = warnings.join('\n');
      notification.style.display = 'block';
    } else {
      notification.style.display = 'none';
    }
  }
});

function fetchSensorData() {
  fetch("/sensor_data")
    .then(response => response.json())
    .then(data => {
      const tempValue = data.temperature; // -10 ~ 40
      const waterLevelValue = data.water_level; // 0 ~ 1000
      const turbidityValue = data.turbidity; // 0 ~ 1000
      const pohValue = data.ph; // 0 ~ 13

      temperature.set((tempValue + 10) / 50); // 온도 -10 ~ 40 -> 0 ~ 1
      water_level.set(waterLevelValue / 1000); // 수위 0 ~ 1000 -> 0 ~ 1
      turbidity.set(turbidityValue / 1000); // 탁도 0 ~ 1000 -> 0 ~ 1
      poh.set(pohValue / 13); // pH 0 ~ 13 -> 0 ~ 1
    });
}

