# -*- encoding: utf-8 -*-

import json
import web.form

from meta import database
from meta.page import Page

class Register(Page):
    """
    Register a new user
    POST /register {
        'email': "email@pif.net", #required
        'fullname': "The full name", #required
        'password': "password', #required
    }

    """

    _validators = {
        'email': web.form.regexp(r".*@.*", "must be a valid email address"),
        'fullname': web.form.regexp(r".{3,90}$", 'fullname must be between 3 and 90 characters'),
        'password': web.form.regexp(r".{3,20}$", 'password must be between 3 and 20 characters'),
    }

    def POST(self):
        if self.user is not None:
            return self.error("Please logout before any register attempt")

        user = self.data
        errors = []
        for k, v in Register._validators.items():
            if not k in user:
                errors.append("Field '%s' is mandatory" % k)
            else:
                f = str(user[k]).strip()
                if not v.valid(f):
                    errors.append(v.msg)
                else:
                    user[k] = f

        if not len(errors) and database.users.find_one({'email': user['email']}):
            errors.append('This email is already registered')
        if len(errors):
            return json.dumps({
                'success': False,
                'errors': errors,
            })

        self.registerUser(
            email=user['email'],
            fullname=user['fullname'],
            password=self.hashPassword(user['password']),
        )
        return json.dumps({
            'success': True,
        })

