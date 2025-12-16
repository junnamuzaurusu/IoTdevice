from services import raw_data
from fastapi import APIRouter

api_router = APIRouter()
api_router.include_router(raw_data.router, prefix=raw_data.prefix)
