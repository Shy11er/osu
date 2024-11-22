import { useEffect, useState } from "react";
import { Chart } from "react-google-charts";

// Функция для генерации случайного числа с нормальным распределением
function generateRandomNumber(mean, variance) {
  let u = Math.random();
  let v = Math.random();
  let z = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);
  return z * Math.sqrt(variance) + mean;
}

/**
 * <h1>Function</h1>
 */
function App() {
  const [sequenceY, setSequenceY] = useState([]);
  const [mean, setMean] = useState(0);
  const [variance, setVariance] = useState(0);
  const [histogramData, setHistogramData] = useState([]);

  useEffect(() => {
    // Генерация последовательности X_i
    const randomNumbers = Array.from({ length: 80 }, () =>
      generateRandomNumber(3, 4)
    );

    // Формирование последовательности Y_i
    let newSequenceY: number[] = [];
    randomNumbers.forEach((x, i) => {
      if (i === 0) {
        newSequenceY.push(x);
      } else {
        newSequenceY.push(newSequenceY[i - 1] + x);
      }
    });

    setSequenceY(newSequenceY);

    // Вычисление среднего значения
    const avg =
      newSequenceY.reduce((sum, y) => sum + y, 0) / newSequenceY.length;
    setMean(avg);

    // Вычисление дисперсии
    const varianceValue =
      newSequenceY.reduce((sum, y) => sum + Math.pow(y - avg, 2), 0) /
      newSequenceY.length;
    setVariance(varianceValue);

    // Подготовка данных для гистограммы
    const min = Math.min(...newSequenceY);
    const max = Math.max(...newSequenceY);
    const range = max - min;
    const step = range / 10;
    const histogram = Array(10).fill(0);

    newSequenceY.forEach((y) => {
      const index = Math.floor((y - min) / step);
      histogram[Math.min(index, 9)]++;
    });

    const histogramData = [["Interval", "Count"]];
    for (let i = 0; i < 10; i++) {
      histogramData.push([
        `${min + i * step}-${min + (i + 1) * step}`,
        histogram[i],
      ]);
    }

    setHistogramData(histogramData);
  }, []);

  console.log(histogramData);
  console.log(mean);
  console.log(variance);
  console.log(sequenceY);

  return (
    <div
      style={{
        display: "flex",
        alignItems: "center",
        justifyContent: "center",
        flexDirection: "column",
        width: "100%",
      }}
    >
      <h1>Генерация последовательности Y</h1>
      <p>Среднее значение: {mean.toFixed(2)}</p>
      <p>Дисперсия: {variance.toFixed(2)}</p>
      <Chart
        chartType="ColumnChart"
        width="100%"
        height="400px"
        data={histogramData}
        options={{
          title: "Гистограмма последовательности Y",
          hAxis: { title: "Интервалы" },
          vAxis: { title: "Количество" },
        }}
      />
    </div>
  );
}

export default App;
