exports.post = function(request, response) {
    // Use "request.service" to access features of your mobile service, e.g.:
    //   var tables = request.service.tables;
    //   var push = request.service.push;

    doWork(request, response);
};

exports.get = function(request, response) {
    response.send(405, "Nur Post erlaubt!");
};

function doWork(request, response) {
    var query = "";
    var requestHeaderTyp = request.header('Content-Type');
    var contentHeaderString = getBodyType(requestHeaderTyp);
    if(contentHeaderString == 1) {
        //Text
        query = JSON.parse(request.body).sqlQuery;
    } else if (contentHeaderString == 2) {
        //JSON
        query = request.body.sqlQuery;
    } else {
        response.send(400, "Content-Typ text/plain oder application/json erwartet! Type war:" +  requestHeaderTyp);
        return;
    }

    //response.send(200, query.sqlQuery);
    //return;
    var mssql = request.service.mssql;
    mssql.query(query,
        {success: function(results) {
            response.send(200, results);
        }
        }
    )
}

function getBodyType(headerContentString) {
    if(headerContentString.beginsWith("text/plain")) {
        return 1;
    }
    if(headerContentString.beginsWith("application/json")) {
        return 2;
    }
}

String.prototype.beginsWith = function (string) {
    return(this.indexOf(string) === 0);
};