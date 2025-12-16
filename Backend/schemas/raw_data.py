from pydantic import BaseModel
from typing import Optional
from datetime import datetime
import json


class RawDataResponse(BaseModel):
    id: str
    mode: int
    date_time: datetime
    illuminance: int
    is_people_detected: bool

    class Config:
        from_attributes = True
    

class PostRawDataRequest(BaseModel):
    mode: int
    date_time: datetime
    illuminance: int
    is_people_detected: bool


    