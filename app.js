var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var router = express.Router();
var fs = require('fs');
var bodyParser = require('body-parser');
var cors = require('cors');

var route_list = fs.readdirSync('./routes');
var dir_list = new Array();
dir_list.push(path.join(__dirname, 'views'));

var app = express();
// view engine setup
// 미들웨어
app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    next();
});

// app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(cors({
    origin: '*',
}));
app.use(express.static(path.join(__dirname, 'public')));

// view import
fs.readdir('./views', function(err, list) {
    //console.log(list);
    for(var i=0; i<list.length; i++) {
        if(!list[i].includes('.')) {
            dir_list.push(path.join(__dirname, 'views/' + list[i] ));
        }
    }
    app.set('views', dir_list);
    app.set('view engine', 'ejs');
});

var indexRouter = require('./routes/index');
app.use('/', indexRouter);

// error handler
app.use(function(err, req, res, next) {
    // set locals, only providing error in development
    res.locals.message = err.message;
    res.locals.error = req.app.get('env') === 'development' ? err : {};
    console.log(err.message);
    // render the error page
    res.status(err.status || 500);
    res.render('error');
});

module.exports = app;
