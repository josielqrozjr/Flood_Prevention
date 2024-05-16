from models.db import db
from models.db import datetime

class Users(db.Model):
    __tablename__ = 'users'

    id= db.Column('id', db.Integer, 
                  nullable=False, 
                  autoincrement=True, 
                  primary_key=True)
    
    name= db.Column(db.String(100), nullable= False)
    cpf= db.Column(db.String(11), nullable= False, unique=True)
    birth_date = db.Column(db.Date(), nullable = False)
    gender= db.Column(db.String(50), nullable= False)
    email= db.Column(db.String(11), nullable= False, unique=True)
    is_active= db.Column(db.Boolean, nullable= False, default= False)
    creation_date = db.Column(db.DateTime(), nullable = False, default=datetime.now())
    update_date = db.Column(db.DateTime(), nullable = False, default=datetime.now())
