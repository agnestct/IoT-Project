// components/FlowSpikeChart.jsx
import React, { useState, useMemo } from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';

class PatternDetector {
  constructor(Nmax = 10, gamma_th = 2.5, sigma2_hat = 0.25) {
    this.Nmax = Nmax;
    this.gamma_th = gamma_th;
    this.sigma2_hat = sigma2_hat;
    this.n_samples = 0;
    this.mu_hat = 0;
    this.state = 'IDLE';
  }

  update(x) {
    const lambda =
      this.n_samples < this.Nmax
        ? 1 / (this.n_samples + 1)
        : 1 / this.Nmax;

    this.mu_hat = (1 - lambda) * this.mu_hat + lambda * x;
    this.n_samples++;

    const T = ((x - this.mu_hat) ** 2) / this.sigma2_hat;
    this.state = T > this.gamma_th ? 'SPIKE' : 'IDLE';

    return { T, state: this.state, mu_hat: this.mu_hat };
  }
}

export default function FlowSpikeChart({ data }) {
  const [intervalMinutes, setIntervalMinutes] = useState(7);
  const [gammaTh, setGammaTh] = useState(250);
  const [sigma2, setSigma2] = useState(1);

  const { chartData, totalSpikes } = useMemo(() => {
    const detector = new PatternDetector(30, gammaTh, sigma2);
    const countMap = {};

    data.forEach(d => {
      const date = new Date(d.timestamp);
      const minutes =
        Math.floor(date.getMinutes() / intervalMinutes) * intervalMinutes;

      const key = `${date.toISOString().slice(0, 13)}:${minutes
        .toString()
        .padStart(2, '0')}`;

      countMap[key] = (countMap[key] || 0) + 1;
    });

    const times = data.map(d => new Date(d.timestamp));
    if (times.length === 0) return { chartData: [], totalSpikes: 0 };

    const startTime = new Date(Math.min(...times));
    const endTime = new Date(Math.max(...times));

    const completeData = [];
    let current = new Date(startTime);
    current.setSeconds(0, 0);

    let spikeCount = 0;

    while (current <= endTime) {
      const minutes =
        Math.floor(current.getMinutes() / intervalMinutes) *
        intervalMinutes;

      const key = `${current.toISOString().slice(0, 13)}:${minutes
        .toString()
        .padStart(2, '0')}`;

      const count = countMap[key] || 0;
      const result = detector.update(count);

      const isSpike = result.state === 'SPIKE' ? 1 : 0;
      if (isSpike) spikeCount++;

      completeData.push({
        time: key,
        count,
        T: result.T,
        spike: isSpike,
      });

      current.setMinutes(current.getMinutes() + intervalMinutes);
    }

    return {
      chartData: completeData,
      totalSpikes: spikeCount,
    };
  }, [data, intervalMinutes, gammaTh, sigma2]);

  return (
    <div style={{ width: '100%', height: 500 }}>
      <div
        style={{
          fontSize: 22,
          fontWeight: 'bold',
          marginBottom: 15,
          color: totalSpikes > 0 ? 'red' : 'black',
        }}
      >
        Total {totalSpikes} {totalSpikes === 1 ? 'Anomaly' : 'Anomalies'} in Flow Count
      </div>

      <div style={{ marginBottom: 20, display: 'flex', gap: 20, justifyContent: 'center', alignItems: 'center' }}>
        <label>
          Interval Minutes:
          <input
            type="number"
            value={intervalMinutes}
            onChange={e =>
              setIntervalMinutes(Math.max(1, Number(e.target.value)))
            }
          />
        </label>

        <label>
          Gamma Threshold:
          <input
            type="number"
            step="10"
            value={gammaTh}
            onChange={e =>
              setGammaTh(Math.max(0, Number(e.target.value)))
            }
          />
        </label>

        <label>
          Sigma²:
          <input
            type="number"
            step="0.1"
            value={sigma2}
            onChange={e =>
              setSigma2(Math.max(0.0001, Number(e.target.value)))
            }
          />
        </label>
      </div>



      <ResponsiveContainer width="100%" height="100%">
        <LineChart data={chartData}>
          <CartesianGrid stroke="#eee" strokeDasharray="5 5" />
          <XAxis dataKey="time" />
          <YAxis yAxisId="left" />
          <YAxis yAxisId="right" orientation="right" />
          <YAxis
            yAxisId="spikeAxis"
            orientation="right"
            domain={[0, 1]}
            tickCount={2}
            tickFormatter={(val) => (val === 1 ? 'Anomaly' : 'Normal')}
          />
          <Tooltip />
          <Legend verticalAlign="bottom" height={36} />
          <Line
            type="monotone"
            dataKey="count"
            stroke="#8884d8"
            yAxisId="left"
            name={`Count per ${intervalMinutes}min`}
          />
          <Line
            type="monotone"
            dataKey="T"
            stroke="red"
            yAxisId="right"
            name="Anomaly Score"
            dot={false}
          />
          <Line
            type="monotone"
            dataKey="spike"
            stroke="green"
            yAxisId="spikeAxis"
            name="Spike"
            dot={{ r: 5 }}
            isAnimationActive={false}
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}
