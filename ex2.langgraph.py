import seaborn as sns
import pandas as pd
from prophet import Prophet
from typing import TypedDict
from langchain.tools import tool
from langgraph.graph import StateGraph, END
from langgraph.prebuilt import ToolNode

# Step 1: Load and preprocess data
flights = sns.load_dataset("flights")
flights['date'] = pd.to_datetime(flights['year'].astype(str) + '-' + flights['month'].astype(str))
flights.set_index('date', inplace=True)

df = flights[['passengers']].rename(columns={'passengers': 'count'})
df.reset_index(inplace=True)

last_n = 30
recent_data = df.tail(last_n)

# Step 2: Define State Schema using TypedDict
class ForecastState(TypedDict):
    forecast: str

# Step 3: Define the forecasting tool using Prophet
@tool
def forecast_with_prophet(_: str = "") -> str:
    """
    Forecasts the next 7 months of counts using Prophet.
    Ignores input string; uses globally defined `recent_data`.
    """
    prophet_df = recent_data.rename(columns={"date": "ds", "count": "y"})

    model = Prophet()
    model.fit(prophet_df)

    future = model.make_future_dataframe(periods=7, freq='M')
    forecast = model.predict(future)

    forecast_df = forecast[['ds', 'yhat']].tail(7)
    forecast_str = "\n".join([
        f"{row['ds'].strftime('%Y-%m-%d')}: {int(row['yhat'])}"
        for _, row in forecast_df.iterrows()
    ])
    return forecast_str

# Step 4: Node to prepare inputs (just sets a dummy input)
def prepare_input_node(state: dict) -> ForecastState:
    return {"forecast": forecast_with_prophet("")}

# Step 5: Build the LangGraph with state schema
builder = StateGraph(ForecastState)

builder.add_node("prepare_input", prepare_input_node)
builder.set_entry_point("prepare_input")
builder.add_edge("prepare_input", END)

app = builder.compile()

# Step 6: Run the graph
result = app.invoke({})
print("Forecast:\n", result["forecast"])
