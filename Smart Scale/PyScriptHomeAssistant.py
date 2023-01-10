import json
import requests
from datetime import datetime


@pyscript_executor
def addEntry(chicken, count, weight):

    # DATABASE_KEY is replaced with the database id of the specific chicken's Notion page.
    chickens = {
        "cleo": "DATABASE_KEY",
        "bella": "DATABASE_KEY",
        "mocha": "DATABASE_KEY",
        "tiffy": "DATABASE_KEY",
        "tara": "DATABASE_KEY",
        "blue": "DATABASE_KEY"
    }

    weight_g = float(weight)
    database = chickens[f"{chicken}"]
    
    # The Notion's autorization secret
    secret = "SECRET"
    # URL
    url = f'https://api.notion.com/v1/pages'
    # Date (without time)
    date = datetime.today().date().isoformat()
    # Headers
    headers = {
        'Authorization': f'Bearer {secret}',
        "Content-Type": "application/json",
        'Notion-Version': '2022-02-22'
    }
    # Data for specific database according to the Notion API:
    data = {
        "parent": {"database_id": database},
        "properties": {
            "Number": {
                "title": [
                    {
                        "text": {
                            "content": f"{count}"
                        }
                    }
                ]
            },
            "Weight in g": {
                "type": "number",
                "number": weight_g
            },
            "Column": {
                "date": {
                    "start": date,
                    "end": None
                }
            },
            "Property": {
                "rich_text": [
                    {
                        "text": {
                            "content": ""
                        }
                    }
                ]
            }
        }}
    data = json.dumps(data)

    res = requests.post(url, headers=headers, data=data)


@service
def notion_cleo():
    weight = state.get(f'input_number.box1')
    count = state.get(f'counter.cleo_egg_counter_total')
    addEntry("cleo", count, weight)


@service
def notion_bella():
    weight = state.get(f'input_number.box1')
    count = state.get(f'counter.bella_egg_counter_total')
    addEntry("bella", count, weight)


@service
def notion_mocha():
    weight = state.get(f'input_number.box1')
    count = state.get(f'counter.mocha_egg_counter_total')
    addEntry("mocha", count, weight)


@service
def notion_tiffy():
    weight = state.get(f'input_number.box1')
    count = state.get(f'counter.tiffy_egg_counter_total')
    addEntry("tiffy", count, weight)


@service
def notion_tara():
    weight = state.get(f'input_number.box1')
    count = state.get(f'counter.tara_egg_counter_total')
    addEntry("tara", count, weight)


@service
def notion_blue():
    weight = state.get(f'input_number.box1')
    count = state.get(f'counter.blue_egg_counter_total')
    addEntry("blue", count, weight)
