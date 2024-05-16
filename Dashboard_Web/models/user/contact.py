from models import db
from models.user.users import Users

class Contact(db.Model):
    __tablename__ = 'contact'

    id= db.Column('id', 
                  db.Integer, 
                  db.ForeignKey(Users.id), 
                  nullable=False, 
                  autoincrement=True, 
                  primary_key=True)

    country_code= db.Column(db.Tinyint(2, unsigned=True, zerofill=True), nullable=False, default='55')
    area_code= db.Column(db.Tinyint(unsigned=True, zerofill=True), nullable=False)
    number= db.Column(db.Integer(unsigned=True), nullable=False)
    users_id = db.Column(db.Integer, db.ForeignKey(Users.id))
