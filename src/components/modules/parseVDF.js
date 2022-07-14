/* eslint-disable no-useless-escape */
/* eslint-disable  no-constant-condition */
/* eslint-disable  no-unused-vars */
const TYPEEX = {
    INT: /^\-?\d+$/,
    FLOAT: /^\-?\d+\.\d+$/,
    BOOLEAN: /^(true|false)$/i,
}

function parse(text, options) {
    options = {
        types:
            (typeof options === "boolean") ? options
            : ((typeof options === "object" && "types" in options) ? options.types : true),
        arrayify: (typeof options === "object" && "arrayify" in options) ? options.arrayify : true,
        conditionals: (typeof options === "object" && "conditionals" in options) ? options.conditionals : undefined
    };
    if (options.conditionals && !Array.isArray(options.conditionals) && typeof options.conditionals === "string") options.conditionals = [options.conditionals];

    let lines = text.split("\n");

    var obj = {};
    var stack = [obj];
    var expect_bracket = false;
    var odd = false;

    var re_kv = new RegExp(
        '^[ \\t]*' +
        '("((?:\\\\.|[^\\\\"])+)"|([a-zA-Z0-9\\-\\_]+))' +
    
        '([ \\t]*(' +
        '"((?:\\\\.|[^\\\\"])*)(")?' +
        '|([a-zA-Z0-9\\-\\_.]+)' +
        '))?' +
    
        '(?:[ \\t]*\\[(\\!?\\$[A-Z0-9]+(?:(?:[\\|]{2}|[\\&]{2})\\!?\\$[A-Z0-9]+)*)\\])?'
    );

    var i = -1, j = lines.length, line, sublines;
    var getNextLine = function() {
        if (sublines && sublines.length)
        {
            var _subline = sublines.shift();
            if (!odd) _subline = _subline.trim(); 
            return _subline;
        }

        var _line = lines[++i];
        
        while ( !odd && _line !== undefined && (_line = _line.trim()) && (_line == "" || _line[0] == '/') )
            _line = lines[++i];

        if (_line === undefined)
            return false; 

        var comment_slash_pos = -1;
        sanitization: for (var l = 0; l < _line.length; l++) {
            switch (_line.charAt(l)) {
                case '"': if (_line.charAt(l-1) != '\\') odd = !odd; break;
                case '/': if (!odd) { comment_slash_pos = l; break sanitization; } break;
                case '{': if (!odd) { _line = _line.slice(0, l) + "\n{\n" + _line.slice(l+1); l+=2; } break;
                case '}': if (!odd) { _line = _line.slice(0, l) + "\n}\n" + _line.slice(l+1); l+=2; } break;
            }
        }
        if (comment_slash_pos > -1) _line = _line.substr(0, comment_slash_pos);

        sublines = _line.split("\n");
        return getNextLine();
    }

    while ((line = getNextLine()) !== false) {

        if ( line == "" || line[0] == '/') { continue; }

        if ( line[0] == "{" ) {
            expect_bracket = false;
            continue;
        }

        if ( line[0] == "}" ) {
            if (Array.isArray(stack[stack.length-2])) stack.pop();
            stack.pop();
            continue;
        }

        while (true) {
            let m = re_kv.exec(line);

            var key = (m[2] !== undefined) ? m[2] : m[3];
            var val = (m[6] !== undefined) ? m[6] : m[8];

            if (val === undefined) {
                if (stack[stack.length-1][key] === undefined) {
                    stack[stack.length-1][key] = {};
                    stack.push(stack[stack.length-1][key]);
                }
                else if (stack[stack.length-1][key] !== undefined && !Array.isArray(stack[stack.length-1][key])) {
                    if (options.arrayify) {
                        stack[stack.length-1][key] = [stack[stack.length-1][key], {}];
                        stack.push(stack[stack.length-1][key]);
                        stack.push(stack[stack.length-1][1]);
                    } else {
                        stack.push(stack[stack.length-1][key]);
                    }
                }

                else if (stack[stack.length-1][key] !== undefined && Array.isArray(stack[stack.length-1][key])) {
                    stack.push(stack[stack.length-1][key]); 
                    stack[stack.length-1].push({});
                    stack.push(stack[stack.length-1][ (stack[stack.length-1]).length-1 ]);
                }

                expect_bracket = true;
            }
            else {
                
                if (m[7] === undefined && m[8] === undefined) {
                    line += "\n" + getNextLine();
                    continue;
                }

                if (options.conditionals !== undefined && Array.isArray(options.conditionals) && m[9]) {
                    var conditionals = m[9];
                    var single_cond_regex = new RegExp('^(\\|\\||&&)?(!)?\\$([A-Z0-9]+)');
                    var ok = false;
                    while (conditionals) {
                        var d = single_cond_regex.exec(conditionals);
                        conditionals = conditionals.replace(d[0], '').trim();
                        var op = d[1];
                        var not = d[2] && d[2] === '!';
                        var cond = d[3];
                        var includes = options.conditionals.indexOf(cond) !== -1;
                        var _ok = not ? !includes : includes;
                        if (!op || op === '||')
                            ok = ok || _ok;
                        else
                            ok = ok && _ok;
                    }
                    if (!ok) {
                        line = line.replace(m[0], "");
                        if (!line || line[0] == '/') break;
                        continue;
                    }
                }
                
                if (options.types) {
                    if (TYPEEX.INT.test(val)) {
                        val = parseInt(val);
                    } else if (TYPEEX.FLOAT.test(val)) {
                        val = parseFloat(val);
                    } else if (TYPEEX.BOOLEAN.test(val)) {
                        val = val.toLowerCase() == "true";
                    }
                }

                if (stack[stack.length-1][key] === undefined) {
                    stack[stack.length-1][key] = val;
                }

                else if (stack[stack.length-1][key] !== undefined && !Array.isArray(stack[stack.length-1][key])) {
                    if (options.arrayify) {
                        stack[stack.length-1][key] = [stack[stack.length-1][key], val];
                    } else {
                        stack[stack.length-1][key] = val;
                    }
                }

                else if (stack[stack.length-1][key] !== undefined && Array.isArray(stack[stack.length-1][key])) {
                    stack[stack.length-1][key].push(val);
                }
                
            }

            if (expect_bracket) break;
            line = line.replace(m[0], "").trim();
            if (!line || line[0] == '/') break; 
            line = line.replace(/^\s*\[\!?\$[A-Z0-9]+(?:(?:[\|]{2}|[\&]{2})\!?\$[A-Z0-9]+)*\]/, "").trim(); 
            if (!line || line[0] == '/') break;
        }
    
    }

    return obj;
}

exports.parse = parse;