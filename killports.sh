lsof -t -i tcp:8000 | xargs kill -9
lsof -t -i tcp:8001 | xargs kill -9
lsof -t -i tcp:8002 | xargs kill -9
