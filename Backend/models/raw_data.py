from sqlalchemy import Column, Integer, String, DateTime, Boolean, CheckConstraint
from sqlalchemy.orm import Mapped, mapped_column 
from datetime import datetime
from settings import Base   

class   RawData(Base):
    __tablename__ = "raw_data"

    id: Mapped[str]  = Column(String(26), primary_key=True)
    mode: Mapped[int] = Column(Integer)
    date_time: Mapped[datetime] = Column(DateTime)
    illuminance: Mapped[int] = Column(Integer)
    is_people_detected: Mapped[bool] = Column(Boolean)
    is_outlier: Mapped[bool] = Column(Boolean)

    __table_args__ = (
        CheckConstraint("mode >= 0 AND mode < 4"),
    )


