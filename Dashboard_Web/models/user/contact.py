from models import db
<<<<<<< Updated upstream
from models.user.users import Users
=======
from models.db import datetime
>>>>>>> Stashed changes

class Contact(db.Model):
    __tablename__ = 'contact'

    id= db.Column('id', 
                  db.Integer, 
                  db.ForeignKey(Users.id), 
                  nullable=False, 
                  autoincrement=True, 
                  primary_key=True)

<<<<<<< Updated upstream
    country_code= db.Column(db.Tinyint(2, unsigned=True, zerofill=True), nullable=False, default='55')
    area_code= db.Column(db.Tinyint(unsigned=True, zerofill=True), nullable=False)
    number= db.Column(db.Integer(unsigned=True), nullable=False)
    users_id = db.Column(db.Integer, db.ForeignKey(Users.id))
=======
    country_code = db.Column(db.SmallInteger, nullable=False, default=55)
    area_code = db.Column(db.SmallInteger, nullable=False)
    number = db.Column(db.Integer, nullable=False)
    update_date = db.Column(db.DateTime, nullable=False, default=datetime.now, onupdate=datetime.now)

    # Foreign keys
    user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)
    
    # Relationship
    user = db.relationship('Users', back_populates='contact')
>>>>>>> Stashed changes
