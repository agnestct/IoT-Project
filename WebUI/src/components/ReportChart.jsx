import { useState } from "react";
import { Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer, ComposedChart, Bar } from "recharts";

function ReportChart({ history }) {
  const chunkSize = 30;
  const [page, setPage] = useState(0);
  const [showHistory, setShowHistory] = useState(false);

  if (!history || history.length === 0) return <p style={{ padding: 15, textAlign: "center" }}>No history data available</p>;

  const maxPage = Math.floor((history.length - 1) / chunkSize);
  const startIndex = history.length - (page + 1) * chunkSize;
  const endIndex = history.length - page * chunkSize;
  const sliceStart = startIndex < 0 ? 0 : startIndex;
  const sliceEnd = endIndex;

  const chartData = history.slice(sliceStart, sliceEnd);
  const historyData = chartData.slice().reverse();

  const floorSummary = history.reduce(
    (acc, item) => {
      if (item.floor >= 2 && item.floor <= 7) acc[item.floor] += 1;
      else acc.other += 1;
      return acc;
    },
    { 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, other: 0 }
  );

  return (
    <div style={{ padding: 15, backgroundColor: "#fff" }}>
      <h2>Report</h2>

      <div style={{ display: "flex", justifyContent: "space-between", marginBottom: 10 }}>
        {Object.keys(floorSummary).map(key => (
          <div key={key} style={{ flex: 1, textAlign: "center", border: "1px solid #ccc", margin: "0 2px", padding: 5 }}>
            <strong>{key}</strong>
            <div>{floorSummary[key]}</div>
          </div>
        ))}
      </div>

      <div style={{ display: "flex", justifyContent: "space-between", alignItems: "center", margin: "10px 0" }}>
        <button disabled={page === 0} onClick={() => setPage(p => p - 1)}>Previous</button>
        <span>Page {page + 1} / {maxPage + 1}</span>
        <button disabled={page >= maxPage} onClick={() => setPage(p => p + 1)}>Next</button>
      </div>

      <h3 style={{ textAlign: "center" }}>Charts</h3>
      <div style={{ height: 300 }}>
        <ResponsiveContainer width="100%" height="100%">
          <ComposedChart data={chartData} margin={{ top: 20, right: 30, left: 0, bottom: 5 }}>
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="timestamp" />
            <YAxis yAxisId="left" orientation="left" allowDecimals={false} domain={[1.5,7.5]} />
            <YAxis
              yAxisId="right"
              orientation="right"
              domain={[
                Math.min(...chartData.map(d => d.pressure)) * 0.9,
                Math.max(...chartData.map(d => d.pressure)) * 1.1
              ]}
              tickFormatter={v => v.toFixed(1)}
            />
            <Tooltip />
            <Legend />
            <Bar dataKey="floor" fill="#8884d8" yAxisId="left" name="Floor" />
            <Line type="monotone" dataKey="pressure" stroke="#82ca9d" yAxisId="right" name="Pressure" />
          </ComposedChart>
        </ResponsiveContainer>
      </div>

      <button onClick={() => setShowHistory(!showHistory)}>
        {showHistory ? "Hide History" : "Show History"}
      </button>

      {showHistory && (
        <div>
          <h3 style={{ textAlign: "center" }}>History</h3>
          {historyData.map((item, index) => (
            <div key={sliceStart + index} style={{ marginBottom: 5 }}>
              {item.timestamp} - Floor: {item.floor}, Pressure: {item.pressure}, Pattern: {item.pattern}
            </div>
          ))}
        </div>
      )}
    </div>
  );
}

export default ReportChart;
